#include "D3DResourceLeakChecker.h"
#include <dxgidebug.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

namespace MyEngine {

    // デストラクタでリソースリークチェックを実行
    D3DResourceLeakChecker::~D3DResourceLeakChecker() {
        Microsoft::WRL::ComPtr<IDXGIDebug1> debug;

        // DXGIのデバッグインターフェースが取得できた場合のみレポート
        if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug)))) {
            // 全体のリソース
            debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
            // アプリケーション側のリソース
            debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
            // D3D12リソース
            debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
        }
    }
}