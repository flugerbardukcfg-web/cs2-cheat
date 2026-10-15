#include "chams.hpp"
#include "../../sdk/source2-sdk/memory_classes/utlbuffer.hpp"
#include <thread>
#include <chrono>
CMaterial2* visible;
CMaterial2* invisible;
CGameEntitySystem entitysystem;

bool chams::init( )
{
	 
	if (chams_init)
		return chams_init;

	chams::invisibleforblackC = chams::create_invisible_for_black("invisible_for_black");
	chams::visibleforblackC = chams::create_visible_for_black("visible_for_black");
	chams::visibleC = chams::create_visible("visible");
	chams::invisibleC = chams::create_invisible("invisible");
	chams::bloomC = chams::create_bloom("bloom");
	chams_init = visibleC && invisibleC && visibleforblackC && invisibleforblackC && bloomC;
	return chams_init;
}

CMaterial2* chams::create_bloom(const char* name)
{
	std::string vmat_buffer =
		R"(<!-- kv3 encoding:text:version{e21c7f3c-8a33-41c5-9977-a76d3a32aa0d} format:generic:version{7412167c-06e9-4698-aff2-e63eb59037e7} -->
{
	shader = "csgo_complex.vfx"

	F_SELF_ILLUM = 1
	F_PAINT_VERTEX_COLORS = 1
	F_TRANSLUCENT = 1
    F_DISABLE_Z_BUFFERING = 1

	g_vColorTint = [ 1.000000, 1.000000, 1.000000, 1.000000 ]
	g_flSelfIllumScale = [ 3.000000, 3.000000, 3.000000, 3.000000 ]
	g_flSelfIllumBrightness = [ 3.000000, 3.000000, 3.000000, 3.000000 ]
    g_vSelfIllumTint = [ 8.000000, 8.000000, 8.000000, 8.000000 ]

	g_tColor = resource:"materials/default/default_mask_tga_fde710a5.vtex"
	g_tNormal = resource:"materials/default/default_mask_tga_fde710a5.vtex"
	g_tSelfIllumMask = resource:"materials/default/default_mask_tga_fde710a5.vtex"
	TextureAmbientOcclusion = resource:"materials/debug/particleerror.vtex"
	g_tAmbientOcclusion = resource:"materials/debug/particleerror.vtex"

})";


	KeyValues3 kv{};
 if (!kv.set_type(kv_basic_array, kv_basic_table) || !kv.LoadFromText(vmat_buffer.c_str())) return nullptr;
	CMaterial2** custom_material = nullptr;
	interfaces::pMaterialSystem->CreateMaterial(&custom_material, name, &kv);

	return custom_material ? *custom_material : nullptr;
}

CMaterial2* chams::create_visible(const char* name)
{
	std::string vmat_buffer =
		R"(<!-- kv3 encoding:text:version{e21c7f3c-8a33-41c5-9977-a76d3a32aa0d} format:generic:version{7412167c-06e9-4698-aff2-e63eb59037e7} -->
{
	shader = "csgo_complex.vfx"

	F_SELF_ILLUM = 1
	F_PAINT_VERTEX_COLORS = 1
	F_TRANSLUCENT = 1

	g_vColorTint = [ 1.000000, 1.000000, 1.000000, 1.000000 ]
	g_flSelfIllumScale = [ 3.000000, 3.000000, 3.000000, 3.000000 ]
	g_flSelfIllumBrightness = [ 3.000000, 3.000000, 3.000000, 3.000000 ]
    g_vSelfIllumTint = [ 8.000000, 8.000000, 8.000000, 8.000000 ]

	g_tColor = resource:"materials/default/default_mask_tga_fde710a5.vtex"
	g_tNormal = resource:"materials/default/default_mask_tga_fde710a5.vtex"
	g_tSelfIllumMask = resource:"materials/default/default_mask_tga_fde710a5.vtex"
	TextureAmbientOcclusion = resource:"materials/debug/particleerror.vtex"
	g_tAmbientOcclusion = resource:"materials/debug/particleerror.vtex"

})";


	KeyValues3 kv{};
 if (!kv.set_type(kv_basic_array, kv_basic_table) || !kv.LoadFromText(vmat_buffer.c_str())) return nullptr;
	CMaterial2** custom_material = nullptr;
	interfaces::pMaterialSystem->CreateMaterial(&custom_material, name, &kv);

	return custom_material ? *custom_material : nullptr;
}

CMaterial2* chams::create_visible_for_black(const char* name)
{
	std::string vmat_buffer =
		R"(<!-- kv3 encoding:text:version{e21c7f3c-8a33-41c5-9977-a76d3a32aa0d} format:generic:version{7412167c-06e9-4698-aff2-e63eb59037e7} -->
{
   shader = "csgo_effects.vfx"

            g_flFresnelExponent = 7.0
            g_flFresnelFalloff = 10.0
            g_flFresnelMax = 0.1
            g_flFresnelMin = 2.0
            F_DISABLE_Z_BUFFERING = 0

            g_tColor = resource:"materials/default/default_mask_tga_fde710a5.vtex"
            g_tMask1 = resource:"materials/default/default_mask_tga_fde710a5.vtex"
            g_tMask2 = resource:"materials/default/default_mask_tga_fde710a5.vtex"
            g_tMask3 = resource:"materials/default/default_mask_tga_fde710a5.vtex"
            g_tSceneDepth = resource:"materials/default/default_mask_tga_fde710a5.vtex"

            g_flToolsVisCubemapReflectionRoughness = 5.0

            g_flBeginMixingRoughness = 4.0

            g_vColorTint = [ 0.350000, 0.350000, 0.350000, 0.800000 ]

})";


	KeyValues3 kv{};
 if (!kv.set_type(kv_basic_array, kv_basic_table) || !kv.LoadFromText(vmat_buffer.c_str())) return nullptr;
	CMaterial2** custom_material = nullptr;
	interfaces::pMaterialSystem->CreateMaterial(&custom_material, name, &kv);

	return custom_material ? *custom_material : nullptr;
}

CMaterial2* chams::create_invisible_for_black(const char* name)
{
	std::string vmat_buffer =
		R"(<!-- kv3 encoding:text:version{e21c7f3c-8a33-41c5-9977-a76d3a32aa0d} format:generic:version{7412167c-06e9-4698-aff2-e63eb59037e7} -->
{
   shader = "csgo_effects.vfx"

            g_flFresnelExponent = 7.0
            g_flFresnelFalloff = 10.0
            g_flFresnelMax = 0.1
            g_flFresnelMin = 2.0
            F_DISABLE_Z_BUFFERING = 1

            g_tColor = resource:"materials/default/default_mask_tga_fde710a5.vtex"
            g_tMask1 = resource:"materials/default/default_mask_tga_fde710a5.vtex"
            g_tMask2 = resource:"materials/default/default_mask_tga_fde710a5.vtex"
            g_tMask3 = resource:"materials/default/default_mask_tga_fde710a5.vtex"
            g_tSceneDepth = resource:"materials/default/default_mask_tga_fde710a5.vtex"

            g_flToolsVisCubemapReflectionRoughness = 5.0

            g_flBeginMixingRoughness = 4.0

            g_vColorTint = [ 0.350000, 0.350000, 0.350000, 0.800000 ]

})";


	KeyValues3 kv{};
 if (!kv.set_type(kv_basic_array, kv_basic_table) || !kv.LoadFromText(vmat_buffer.c_str())) return nullptr;
	CMaterial2** custom_material = nullptr;
	interfaces::pMaterialSystem->CreateMaterial(&custom_material, name, &kv);

	return custom_material ? *custom_material : nullptr;
}


CMaterial2* chams::create_invisible(const char* name)
{
	std::string vmat_buffer =
		R"(<!-- kv3 encoding:text:version{e21c7f3c-8a33-41c5-9977-a76d3a32aa0d} format:generic:version{7412167c-06e9-4698-aff2-e63eb59037e7} -->
{
	shader = "csgo_complex.vfx"

	F_SELF_ILLUM = 1
	F_PAINT_VERTEX_COLORS = 1
	F_TRANSLUCENT = 1
    F_DISABLE_Z_BUFFERING = 1

	g_vColorTint = [ 1.000000, 1.000000, 1.000000, 1.000000 ]
	g_flSelfIllumScale = [ 5.000000, 5.000000, 5.000000, 5.000000 ]
	g_flSelfIllumBrightness = [ 3.000000, 3.000000, 3.000000, 3.000000 ]
    g_vSelfIllumTint = [ 8.000000, 8.000000, 8.000000, 8.000000 ]

	g_tColor = resource:"materials/default/default_mask_tga_fde710a5.vtex"
	g_tNormal = resource:"materials/default/default_mask_tga_fde710a5.vtex"
	g_tSelfIllumMask = resource:"materials/default/default_mask_tga_fde710a5.vtex"
	TextureAmbientOcclusion = resource:"materials/debug/particleerror.vtex"
	g_tAmbientOcclusion = resource:"materials/debug/particleerror.vtex"

})";


	KeyValues3 kv{};
 if (!kv.set_type(kv_basic_array, kv_basic_table) || !kv.LoadFromText(vmat_buffer.c_str())) return nullptr;
	CMaterial2** custom_material = nullptr;
	interfaces::pMaterialSystem->CreateMaterial(&custom_material, name, &kv);

	return custom_material ? *custom_material : nullptr;
}
