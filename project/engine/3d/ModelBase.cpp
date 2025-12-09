#include "ModelBase.h"

namespace MyEngine {

	void ModelBase::Initialize(DirectXBase* directx_base) {
		// DirectXBaseのポインタを保持
		directx_base_ = directx_base;
	}
}