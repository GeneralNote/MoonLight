#ifndef __MOONLIGHT_PTR_H__
#define __MOONLIGHT_PTR_H__

#include <wrl/client.h>

namespace ml
{
	template <typename T>
	using Ptr = Microsoft::WRL::ComPtr<T>;
}

#endif // !__MOONLIGHT_PTR_H__
