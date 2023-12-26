// gml_interop.h: Exposes special functions for interoperation with GameMaker Language.

#pragma once
namespace gml
{
	typedef int32_t ds_map;
	typedef void(*event_perform_async_t)(ds_map map, int32_t event_type);
	typedef ds_map(*ds_map_create_ext_t)(int32_t n, ...);
	typedef bool(*ds_map_set_double_t)(ds_map map, const char* key, double value);
	typedef bool(*ds_map_set_string_t)(ds_map map, const char* key, const char* value);

	// see https://web.archive.org/web/20160303070839/https://help.yoyogames.com/hc/en-us/articles/216755258-Returning-Values-From-An-Extension-Asynchronously-GMS-v1-3-

	extern event_perform_async_t event_perform_async;
	extern ds_map_create_ext_t ds_map_create_ext;
	extern ds_map_set_double_t ds_map_set_double;
	extern ds_map_set_string_t ds_map_set_string;

	inline ds_map ds_map_create()
	{
		return ds_map_create_ext(0);
	}

	const int32_t EVENT_OTHER_SOCIAL = 70;
	const int32_t EVENT_OTHER_ASYNC_SAVE_LOAD = 72;
}
