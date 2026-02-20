#include "DirectXBase.h"
#include "Logger.h"
#include "StringUtility.h"
#include <cassert>
#include <format>
#include "externals/DirectXTex/d3dx12.h"
#include <thread>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

namespace MyEngine {

	using namespace Microsoft::WRL;

	std::unique_ptr<DirectXBase> DirectXBase::instance_ = nullptr;

	// シングルトンインスタンス
	DirectXBase* DirectXBase::GetInstance() {
		if (!instance_) {
			instance_ = std::make_unique<DirectXBase>();
		}
		return instance_.get();
	}

	void DirectXBase::Initialize(WindowsApi* windows_api) {
		// NULL検出
		assert(windows_api);
		// メンバ変数に記録
		windows_api_ = windows_api;

		pso_ = std::make_unique<PipelineStateObject>();
		pso_->Initialize(this);

		// FPS固定初期化
		InitializeFixFPS();
		// デバイスの生成
		DeviceInitialize();
		// コマンド関連の初期化
		CommandInitialize();
		// スワップチェーンの生成
		SwapchainGenerate();
		// 深度バッファの生成
		DepthbufferGenerate();
		// デスクリプタヒープの生成
		DescriptorheapGenerate();
		// レンダーターゲットビューの初期化
		RendertargetviewInitialize();
		// 深度ステンシルビューの初期化
		DepthstencilviewInitialize();
		// フェンスの初期化
		FenceInitialize();
		// ビューポート矩形の初期化
		ViewportInitialize();
		// シザリング矩形の初期化
		ScissorrectInitialize();
		// DXCコンパイラの生成
		DXCompilerInitialize();
		// グラフィックスパイプライン作成
		pso_->CreatePostEffectPipelineState();
	}

	// 終了
	void DirectXBase::Finalize() {
		CloseHandle(fence_event_);
		instance_.reset();
	}

	// デバイスの初期化
	void DirectXBase::DeviceInitialize() {

#ifdef _DEBUG
		ComPtr<ID3D12Debug1> debug_controller = nullptr;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug_controller)))) {
			debug_controller->EnableDebugLayer();
			debug_controller->SetEnableGPUBasedValidation(TRUE);
		}
#endif

		hr_ = CreateDXGIFactory(IID_PPV_ARGS(&dxgi_factory_));
		assert(SUCCEEDED(hr_));

		ComPtr<IDXGIAdapter4> use_adapter = nullptr;

		for (UINT i = 0;
			dxgi_factory_->EnumAdapterByGpuPreference(
				i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&use_adapter))
			!= DXGI_ERROR_NOT_FOUND;
			i++)
		{
			DXGI_ADAPTER_DESC3 adapter_desc{};
			hr_ = use_adapter->GetDesc3(&adapter_desc);
			assert(SUCCEEDED(hr_));

			if (!(adapter_desc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
				Logger::Log(string_utility::ConvertString(
					std::format(L"Use Adapter:{}\n", adapter_desc.Description)));
				break;
			}
			use_adapter = nullptr;
		}

		assert(use_adapter != nullptr);

		D3D_FEATURE_LEVEL feature_levels[] = {
			D3D_FEATURE_LEVEL_12_2,
			D3D_FEATURE_LEVEL_12_1,
			D3D_FEATURE_LEVEL_12_0
		};
		const char* level_strings[] = { "12.2", "12.1", "12.0" };

		for (size_t i = 0; i < _countof(feature_levels); ++i) {
			hr_ = D3D12CreateDevice(
				use_adapter.Get(), feature_levels[i], IID_PPV_ARGS(&device_));
			if (SUCCEEDED(hr_)) {
				Logger::Log(std::format("FeatureLevel : {}\n", level_strings[i]));
				break;
			}
		}

		assert(device_ != nullptr);
		Logger::Log("complete create D3D12Device!!!\n");

#ifdef _DEBUG
		ComPtr<ID3D12InfoQueue> info_queue = nullptr;
		if (SUCCEEDED(device_->QueryInterface(IID_PPV_ARGS(&info_queue)))) {
			info_queue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
			info_queue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
			info_queue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);

			D3D12_MESSAGE_ID deny_ids[] = {
				D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
			};
			D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };

			D3D12_INFO_QUEUE_FILTER filter{};
			filter.DenyList.NumIDs = _countof(deny_ids);
			filter.DenyList.pIDList = deny_ids;
			filter.DenyList.NumSeverities = _countof(severities);
			filter.DenyList.pSeverityList = severities;

			info_queue->PushStorageFilter(&filter);
		}
#endif
	}

	// コマンド関連の初期化
	void DirectXBase::CommandInitialize() {
		hr_ = device_->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&command_allocator_));
		assert(SUCCEEDED(hr_));

		hr_ = device_->CreateCommandList(
			0, D3D12_COMMAND_LIST_TYPE_DIRECT,
			command_allocator_.Get(), nullptr,
			IID_PPV_ARGS(&command_list_));
		assert(SUCCEEDED(hr_));

		D3D12_COMMAND_QUEUE_DESC queue_desc{};
		hr_ = device_->CreateCommandQueue(
			&queue_desc, IID_PPV_ARGS(&command_queue_));
		assert(SUCCEEDED(hr_));
	}

	// スワップチェーンの生成
	void DirectXBase::SwapchainGenerate() {
		swap_chain_desc_.Width = WindowsApi::kClientWidth;
		swap_chain_desc_.Height = WindowsApi::kClientHeight;
		swap_chain_desc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swap_chain_desc_.SampleDesc.Count = 1;
		swap_chain_desc_.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swap_chain_desc_.BufferCount = 2;
		swap_chain_desc_.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

		hr_ = dxgi_factory_->CreateSwapChainForHwnd(
			command_queue_.Get(),
			windows_api_->GetHwnd(),
			&swap_chain_desc_,
			nullptr, nullptr,
			reinterpret_cast<IDXGISwapChain1**>(swap_chain_.GetAddressOf()));
		assert(SUCCEEDED(hr_));
	}

	// 深度バッファの生成
	void DirectXBase::DepthbufferGenerate() {

		D3D12_RESOURCE_DESC resource_desc{};
		resource_desc.Width = windows_api_->kClientWidth;
		resource_desc.Height = windows_api_->kClientHeight;
		resource_desc.MipLevels = 1;
		resource_desc.DepthOrArraySize = 1;
		resource_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		resource_desc.SampleDesc.Count = 1;
		resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		resource_desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		D3D12_HEAP_PROPERTIES heap_props{};
		heap_props.Type = D3D12_HEAP_TYPE_DEFAULT;

		D3D12_CLEAR_VALUE depth_clear{};
		depth_clear.DepthStencil.Depth = 1.0f;
		depth_clear.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

		hr_ = device_->CreateCommittedResource(
			&heap_props,
			D3D12_HEAP_FLAG_NONE,
			&resource_desc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			&depth_clear,
			IID_PPV_ARGS(&resource_));
		assert(SUCCEEDED(hr_));
	}
	//デスクリプタヒープの生成
	void DirectXBase::DescriptorheapGenerate() {
		//DescriptorSizeを取得しておく
		descriptor_size_rtv_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		descriptor_size_dsv_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
		descriptor_size_srv_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		// RTV用のディスクリプタヒープの生成
		rtv_descriptor_heap_ = CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 2, false);
		// DSV用のディスクリプタヒープの作成
		dsv_descriptor_heap_ = CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, false);
		//SRV用のディスクリプタヒープの作成
		srv_descriptor_heap_ = CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, kMaxSrvCount, true);

		srv_heap_desc_.NumDescriptors = 1;
		srv_heap_desc_.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		srv_heap_desc_.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

		hr_ = device_->CreateDescriptorHeap(&srv_heap_desc_, IID_PPV_ARGS(&srv_descriptor_heap_));
		assert(SUCCEEDED(hr_));
	}

	//レンダーターゲットビューの初期化
	void DirectXBase::RendertargetviewInitialize() {
		//SwapChainからResourceを引っ張ってくる
		hr_ = swap_chain_->GetBuffer(0, IID_PPV_ARGS(&swap_chain_resources_[0]));
		assert(SUCCEEDED(hr_));
		hr_ = swap_chain_->GetBuffer(1, IID_PPV_ARGS(&swap_chain_resources_[1]));
		assert(SUCCEEDED(hr_));

		D3D12_DESCRIPTOR_HEAP_DESC rtv_heap_desc{};
		rtv_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtv_heap_desc.NumDescriptors = 3;
		rtv_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		rtv_heap_desc.NodeMask = 0;

		device_->CreateDescriptorHeap(&rtv_heap_desc, IID_PPV_ARGS(&rtv_descriptor_heap_));

		rtv_desc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		rtv_desc_.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

		D3D12_CPU_DESCRIPTOR_HANDLE rtv_start = GetCPUDescriptorHandle(rtv_descriptor_heap_, descriptor_size_rtv_, 0);

		//1つ目
		rtv_handles_[0] = rtv_start;
		device_->CreateRenderTargetView(swap_chain_resources_[0].Get(), &rtv_desc_, rtv_handles_[0]);

		//2つ目
		rtv_handles_[1].ptr = rtv_handles_[0].ptr + device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		device_->CreateRenderTargetView(swap_chain_resources_[1].Get(), &rtv_desc_, rtv_handles_[1]);

		const Vector4 kRenderTargetClearValue{ 1.0f,0.0f,0.0f,1.0f };
		render_texture_resource_ = CreateRenderTextureResource(
			device_, WindowsApi::kClientWidth, WindowsApi::kClientHeight,
			DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, kRenderTargetClearValue);

		render_rtv_descriptor_heap_ = CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 1, false);
		D3D12_CPU_DESCRIPTOR_HANDLE render_handle = render_rtv_descriptor_heap_->GetCPUDescriptorHandleForHeapStart();
		device_->CreateRenderTargetView(render_texture_resource_.Get(), &rtv_desc_, render_handle);

		//レンダーテクスチャのSRV
		render_texture_srv_desc_.Format = render_texture_resource_->GetDesc().Format;
		render_texture_srv_desc_.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		render_texture_srv_desc_.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		render_texture_srv_desc_.Texture2D.MipLevels = 1;

		device_->CreateShaderResourceView(
			render_texture_resource_.Get(), &render_texture_srv_desc_,
			srv_descriptor_heap_->GetCPUDescriptorHandleForHeapStart());
	}

	//深度ステンシルビューの初期化
	void DirectXBase::DepthstencilviewInitialize() {
		depth_stencil_resource_ =
			CreateDepthStencilTextureResource(device_, WindowsApi::kClientWidth, WindowsApi::kClientHeight);

		D3D12_DEPTH_STENCIL_VIEW_DESC dsv_desc{};
		dsv_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsv_desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;

		device_->CreateDepthStencilView(
			depth_stencil_resource_.Get(), &dsv_desc,
			dsv_descriptor_heap_->GetCPUDescriptorHandleForHeapStart());
	}

	//フェンスの初期化
	void DirectXBase::FenceInitialize() {
		hr_ = device_->CreateFence(fence_value_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
		assert(SUCCEEDED(hr_));

		fence_event_ = CreateEvent(NULL, FALSE, FALSE, NULL);
		assert(fence_event_ != nullptr);
	}

	//ビューポート矩形の初期化
	void DirectXBase::ViewportInitialize() {
		viewport_.Width = windows_api_->kClientWidth;
		viewport_.Height = windows_api_->kClientHeight;
		viewport_.TopLeftX = 0;
		viewport_.TopLeftY = 0;
		viewport_.MinDepth = 0.0f;
		viewport_.MaxDepth = 1.0f;
	}

	//シザリング矩形の初期化
	void DirectXBase::ScissorrectInitialize() {
		scissor_rect_.left = 0;
		scissor_rect_.right = windows_api_->kClientWidth;
		scissor_rect_.top = 0;
		scissor_rect_.bottom = windows_api_->kClientHeight;
	}

	//DXCコンパイラの初期化
	void DirectXBase::DXCompilerInitialize() {
		hr_ = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxc_utils_));
		assert(SUCCEEDED(hr_));
		hr_ = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxc_compiler_));
		assert(SUCCEEDED(hr_));

		hr_ = dxc_utils_->CreateDefaultIncludeHandler(&include_handler_);
		assert(SUCCEEDED(hr_));
	}

	//FPS初期化
	void DirectXBase::InitializeFixFPS() {
		reference_ = std::chrono::steady_clock::now();
	}

	//FPS更新
	void DirectXBase::UpdateFixFPS() {
		const std::chrono::microseconds kMinTime(uint64_t(1000000.0f / 60.0f));
		const std::chrono::microseconds kMinCheckTime(uint64_t(1000000.0f / 65.0f));

		std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

		std::chrono::microseconds elapsed =
			std::chrono::duration_cast<std::chrono::microseconds>(now - reference_);

		if (elapsed < kMinCheckTime) {
			while (std::chrono::steady_clock::now() - reference_ < kMinTime) {
				std::this_thread::sleep_for(std::chrono::microseconds(1));
			}
		}

		reference_ = std::chrono::steady_clock::now();
	}
	//描画前処理
	void DirectXBase::PreDraw() {
		//書き込むバックバッファのインデックスの取得
		UINT back_buffer_index = swap_chain_->GetCurrentBackBufferIndex();

		//今回のバリアはTransition
		barrier_.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier_.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier_.Transition.pResource = swap_chain_resources_[back_buffer_index].Get();
		barrier_.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		barrier_.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

		//TransitionBarrierを張る
		command_list_->ResourceBarrier(1, &barrier_);

		//描画先のRTVとDSVを設定する
		D3D12_CPU_DESCRIPTOR_HANDLE dsv_handle = GetDSVCPUDescriptorHandle(0);
		command_list_->OMSetRenderTargets(1, &rtv_handles_[back_buffer_index], false, &dsv_handle);

		//指定した色で画面全体をクリアする
		float clear_color[] = { 0.1f, 0.25f, 0.5f, 1.0f };
		command_list_->ClearRenderTargetView(rtv_handles_[back_buffer_index], clear_color, 0, nullptr);

		//指定した深度で画面全体をクリアする
		command_list_->ClearDepthStencilView(dsv_handle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

		command_list_->RSSetViewports(1, &viewport_);
		command_list_->RSSetScissorRects(1, &scissor_rect_);
	}

	//描画後処理
	void DirectXBase::PostDraw() {
		UINT back_buffer_index = swap_chain_->GetCurrentBackBufferIndex();

		barrier_.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier_.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier_.Transition.pResource = swap_chain_resources_[back_buffer_index].Get();
		barrier_.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrier_.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

		command_list_->ResourceBarrier(1, &barrier_);

		//コマンドリストの内容を確定
		hr_ = command_list_->Close();
		assert(SUCCEEDED(hr_));

		//GPU にコマンドリストを実行させる
		ID3D12CommandList* command_lists[] = { command_list_.Get() };
		command_queue_->ExecuteCommandLists(1, command_lists);

		//画面の交換
		swap_chain_->Present(1, 0);

		//Fence の更新
		fence_value_++;

		//GPU がここまで到達したときに値を書き込む
		command_queue_->Signal(fence_.Get(), fence_value_);

		if (fence_->GetCompletedValue() < fence_value_) {
			fence_->SetEventOnCompletion(fence_value_, fence_event_);
			WaitForSingleObject(fence_event_, INFINITE);
		}

		//FPS固定
		UpdateFixFPS();

		//次のフレーム用にコマンドリストを準備
		hr_ = command_allocator_->Reset();
		assert(SUCCEEDED(hr_));
		hr_ = command_list_->Reset(command_allocator_.Get(), nullptr);
		assert(SUCCEEDED(hr_));
	}

	//RenderTexture描画前処理
	void DirectXBase::PreDrawRenderTexture() {
		if (render_texture_state_ == RenderTextureState::PixelShaderResource) {
			D3D12_RESOURCE_BARRIER barrier{};
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			barrier.Transition.pResource = render_texture_resource_.Get();
			barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
			barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
			barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

			command_list_->ResourceBarrier(1, &barrier);

			render_texture_state_ = RenderTextureState::RenderTarget;
		}

		D3D12_CPU_DESCRIPTOR_HANDLE rtv_handle = render_rtv_descriptor_heap_->GetCPUDescriptorHandleForHeapStart();
		D3D12_CPU_DESCRIPTOR_HANDLE dsv_handle = dsv_descriptor_heap_->GetCPUDescriptorHandleForHeapStart();

		command_list_->OMSetRenderTargets(1, &rtv_handle, FALSE, &dsv_handle);

		float clear_color[] = { 1.0f, 0.0f, 0.0f, 1.0f };
		command_list_->ClearRenderTargetView(rtv_handle, clear_color, 0, nullptr);
		command_list_->ClearDepthStencilView(dsv_handle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

		command_list_->RSSetViewports(1, &viewport_);
		command_list_->RSSetScissorRects(1, &scissor_rect_);
	}

	//Swapchainに描画
	void DirectXBase::DrawRenderTextureToScreen() {
		ID3D12DescriptorHeap* descriptor_heaps[] = { srv_descriptor_heap_.Get() };
		command_list_->SetDescriptorHeaps(1, descriptor_heaps);

		command_list_->SetGraphicsRootSignature(pso_->GetPostEffectRootSignature());
		command_list_->SetPipelineState(pso_->GetPostEffectGraphicsPipelineState());

		command_list_->SetGraphicsRootDescriptorTable(
			2,
			GetGPUDescriptorHandle(srv_descriptor_heap_, descriptor_size_srv_, 0)
		);

		command_list_->DrawInstanced(3, 1, 0, 0);
	}

	//RenderTexture を SRV 用に切り替え
	void DirectXBase::TransitionRenderTextureToSRV() {
		if (render_texture_state_ == RenderTextureState::RenderTarget) {
			D3D12_RESOURCE_BARRIER barrier{};
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			barrier.Transition.pResource = render_texture_resource_.Get();
			barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
			barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

			command_list_->ResourceBarrier(1, &barrier);

			render_texture_state_ = RenderTextureState::PixelShaderResource;
		}
	}

	//テクスチャデータの転送
	Microsoft::WRL::ComPtr<ID3D12Resource>
		DirectXBase::UploadTextureData(
			Microsoft::WRL::ComPtr<ID3D12Resource> texture,
			const DirectX::ScratchImage& mip_images
		) {
		std::vector<D3D12_SUBRESOURCE_DATA> subresources;

		DirectX::PrepareUpload(
			device_.Get(),
			mip_images.GetImages(),
			mip_images.GetImageCount(),
			mip_images.GetMetadata(),
			subresources
		);

		uint64_t intermediate_size =
			GetRequiredIntermediateSize(texture.Get(), 0, UINT(subresources.size()));

		Microsoft::WRL::ComPtr<ID3D12Resource> intermediate_resource =
			CreateBufferResource(intermediate_size);

		UpdateSubresources(
			command_list_.Get(),
			texture.Get(),
			intermediate_resource.Get(),
			0, 0,
			UINT(subresources.size()),
			subresources.data()
		);

		D3D12_RESOURCE_BARRIER barrier{};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = texture.Get();
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;

		command_list_->ResourceBarrier(1, &barrier);

		return intermediate_resource;
	}

	//デスクリプタヒープの生成
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>
		DirectXBase::CreateDescriptorHeap(
			D3D12_DESCRIPTOR_HEAP_TYPE heap_type,
			UINT num_descriptors,
			bool shader_visible
		) {
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptor_heap = nullptr;

		D3D12_DESCRIPTOR_HEAP_DESC heap_desc{};
		heap_desc.Type = heap_type;
		heap_desc.NumDescriptors = num_descriptors;
		heap_desc.Flags = shader_visible ?
			D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE :
			D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

		hr_ = device_->CreateDescriptorHeap(&heap_desc, IID_PPV_ARGS(&descriptor_heap));
		assert(SUCCEEDED(hr_));

		return descriptor_heap;
	}

	// 深度ステンシル用テクスチャリソースの生成
	Microsoft::WRL::ComPtr<ID3D12Resource> DirectXBase::CreateDepthStencilTextureResource(Microsoft::WRL::ComPtr<ID3D12Device> device, int32_t width, int32_t height) {
		// 生成するResourceの設定
		D3D12_RESOURCE_DESC resource_desc{};
		resource_desc.Width = width; // Textureの幅
		resource_desc.Height = height; // Textureの高さ
		resource_desc.MipLevels = 1; // mipmapの数
		resource_desc.DepthOrArraySize = 1; // 奥行きor配列Textureの配列数
		resource_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // DepthStencilとして利用可能なフォーマット
		resource_desc.SampleDesc.Count = 1; // サンプリングカウント。1固定
		resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D; // 2次元
		resource_desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL; // DepthStencilとして使う

		// 利用するHeapの設定
		D3D12_HEAP_PROPERTIES heap_properties{};
		heap_properties.Type = D3D12_HEAP_TYPE_DEFAULT; // VRAM上に作る

		// 深度値のクリア設定
		D3D12_CLEAR_VALUE depth_clear_value{};
		depth_clear_value.DepthStencil.Depth = 1.0f; // 最大値
		depth_clear_value.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // フォーマット。Resource合わせる

		// Resource生成
		HRESULT hr = device->CreateCommittedResource(
			&heap_properties,
			D3D12_HEAP_FLAG_NONE,
			&resource_desc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			&depth_clear_value,
			IID_PPV_ARGS(&resource_));
		assert(SUCCEEDED(hr));
		return resource_;
	}

	// シェーダーのコンパイル
	Microsoft::WRL::ComPtr<IDxcBlob> DirectXBase::CompileShader(const std::wstring& file_path, const wchar_t* profile) {
		// シェーダーコンパイル開始ログ
		Logger::Log(string_utility::ConvertString(std::format(L"Begin CompileShader,path:{},profile:{}\n", file_path, profile)));
		// hlslファイルを読む
		IDxcBlobEncoding* shader_source = nullptr;
		HRESULT hr = dxc_utils_->LoadFile(file_path.c_str(), nullptr, &shader_source);
		assert(SUCCEEDED(hr));
		// 読み込んだファイルの内容を設定
		DxcBuffer shader_source_buffer;
		shader_source_buffer.Ptr = shader_source->GetBufferPointer();
		shader_source_buffer.Size = shader_source->GetBufferSize();
		shader_source_buffer.Encoding = DXC_CP_UTF8;

		// コンパイルオプション設定
		LPCWSTR arguments[] = {
			file_path.c_str(),
			L"-E", L"main",
			L"-T", profile,
			L"-Zi", L"-Qembed_debug",
			L"-Od",
			L"-Zpr",
		};
		// シェーダーをコンパイル
		IDxcResult* shader_result = nullptr;
		hr = dxc_compiler_->Compile(
			&shader_source_buffer,
			arguments,
			_countof(arguments),
			include_handler_.Get(),
			IID_PPV_ARGS(&shader_result)
		);
		assert(SUCCEEDED(hr));

		// 警告・エラーが出ていないか確認
		IDxcBlobUtf8* shader_error = nullptr;
		shader_result->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shader_error), nullptr);
		if (shader_error != nullptr && shader_error->GetStringLength() != 0) {
			Logger::Log(shader_error->GetStringPointer());
			assert(false);
		}

		// コンパイル結果を受け取って返す
		IDxcBlob* shader_blob = nullptr;
		hr = shader_result->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shader_blob), nullptr);
		assert(SUCCEEDED(hr));
		// 成功ログ
		Logger::Log(string_utility::ConvertString(std::format(L"Compile Succeeded,path:{},profile:{}\n", file_path, profile)));
		return shader_blob;
	}

	// バッファリソースの生成
	Microsoft::WRL::ComPtr<ID3D12Resource> DirectXBase::CreateBufferResource(size_t size_in_bytes) {
		// UploadHeapの設定
		D3D12_HEAP_PROPERTIES upload_heap_properties{};
		upload_heap_properties.Type = D3D12_HEAP_TYPE_UPLOAD;
		// バッファリソースの設定
		D3D12_RESOURCE_DESC vertex_resource_desc{};
		vertex_resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		vertex_resource_desc.Width = size_in_bytes;
		vertex_resource_desc.Height = 1;
		vertex_resource_desc.DepthOrArraySize = 1;
		vertex_resource_desc.MipLevels = 1;
		vertex_resource_desc.SampleDesc.Count = 1;
		vertex_resource_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		// 頂点リソース生成
		HRESULT hr = device_->CreateCommittedResource(&upload_heap_properties, D3D12_HEAP_FLAG_NONE,
			&vertex_resource_desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
			IID_PPV_ARGS(&vertex_resource_));
		assert(SUCCEEDED(hr));
		return vertex_resource_;
	}

	// テクスチャリソースの生成
	Microsoft::WRL::ComPtr<ID3D12Resource> DirectXBase::CreateTextureResource(const DirectX::TexMetadata& metadata) {
		// Resourceの設定
		D3D12_RESOURCE_DESC resource_desc{};
		resource_desc.Width = UINT(metadata.width);
		resource_desc.Height = UINT(metadata.height);
		resource_desc.MipLevels = UINT16(metadata.mipLevels);
		resource_desc.DepthOrArraySize = UINT16(metadata.arraySize);
		resource_desc.Format = metadata.format;
		resource_desc.SampleDesc.Count = 1;
		resource_desc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension);

		// Heapの設定
		D3D12_HEAP_PROPERTIES heap_properties{};
		heap_properties.Type = D3D12_HEAP_TYPE_DEFAULT;
		heap_properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heap_properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

		// Resource生成
		HRESULT hr = device_->CreateCommittedResource(
			&heap_properties,
			D3D12_HEAP_FLAG_NONE,
			&resource_desc,
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(&resource_));
		assert(SUCCEEDED(hr));
		return resource_;
	}

	// レンダーテクスチャの生成
	Microsoft::WRL::ComPtr<ID3D12Resource> DirectXBase::CreateRenderTextureResource(Microsoft::WRL::ComPtr<ID3D12Device> device,
		uint32_t width, uint32_t height, DXGI_FORMAT format, const Vector4& clear_color) {

		// Resourceの設定
		D3D12_RESOURCE_DESC resource_desc{};
		resource_desc.Width = width;
		resource_desc.Height = height;
		resource_desc.MipLevels = 1;
		resource_desc.DepthOrArraySize = 1;
		resource_desc.Format = format;
		resource_desc.SampleDesc.Count = 1;
		resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		resource_desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

		// Heapの設定
		D3D12_HEAP_PROPERTIES heap_properties{};
		heap_properties.Type = D3D12_HEAP_TYPE_DEFAULT;

		// ClearValueの設定
		D3D12_CLEAR_VALUE clear_value;
		clear_value.Format = format;
		clear_value.Color[0] = clear_color.x;
		clear_value.Color[1] = clear_color.y;
		clear_value.Color[2] = clear_color.z;
		clear_value.Color[3] = clear_color.w;

		// Resource生成
		device->CreateCommittedResource(&heap_properties, D3D12_HEAP_FLAG_NONE,
			&resource_desc, D3D12_RESOURCE_STATE_RENDER_TARGET,
			&clear_value, IID_PPV_ARGS(&resource_));
		return resource_;
	}

	// テクスチャファイルの読み込み
	DirectX::ScratchImage DirectXBase::LoadTexture(const std::string& file_path) {
		// テクスチャファイルを読み込む
		DirectX::ScratchImage image{};
		std::wstring file_path_w = string_utility::ConvertString(file_path);
		HRESULT hr = DirectX::LoadFromWICFile(file_path_w.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
		assert(SUCCEEDED(hr));
		// ミップマップの作成
		DirectX::ScratchImage mip_images{};
		hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mip_images);
		assert(SUCCEEDED(hr));
		// ミップマップ付きのデータを返す
		return mip_images;
	}

	// CPUデスクリプタハンドル取得関数
	D3D12_CPU_DESCRIPTOR_HANDLE DirectXBase::GetCPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& descriptor_heap, uint32_t descriptor_size, uint32_t index) {
		// デスクリプタヒープからCPUハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE handle_cpu = descriptor_heap->GetCPUDescriptorHandleForHeapStart();
		handle_cpu.ptr += (descriptor_size * index);
		return handle_cpu;
	}

	// GPUデスクリプタハンドル取得関数
	D3D12_GPU_DESCRIPTOR_HANDLE DirectXBase::GetGPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& descriptor_heap, uint32_t descriptor_size, uint32_t index) {
		// デスクリプタヒープからGPUハンドルを取得
		D3D12_GPU_DESCRIPTOR_HANDLE handle_gpu = descriptor_heap->GetGPUDescriptorHandleForHeapStart();
		handle_gpu.ptr += (descriptor_size * index);
		return handle_gpu;
	}

	// DSV CPUデスクリプタハンドル取得関数
	D3D12_CPU_DESCRIPTOR_HANDLE DirectXBase::GetDSVCPUDescriptorHandle(uint32_t index) {
		// DSV用のCPUデスクリプタハンドルを取得
		return GetCPUDescriptorHandle(dsv_descriptor_heap_, descriptor_size_dsv_, index);
	}

	// DSV GPUデスクリプタハンドル取得関数
	D3D12_GPU_DESCRIPTOR_HANDLE DirectXBase::GetDSVGPUDescriptorHandle(uint32_t index) {
		// DSV用のGPUデスクリプタハンドルを取得
		return GetGPUDescriptorHandle(dsv_descriptor_heap_, descriptor_size_dsv_, index);
	}
}