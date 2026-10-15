#include "../../sdk/interfaces/interfaces.hpp"
#include "nightmode.hpp"

void nightmode::run() {


    if (!interfaces::pEngine->IsConnected() ||
        !interfaces::pEngine->IsInGame())
        return;

    CCSPlayerController* Localcontroller =
        CGameEntitySystem::Get().GetLocalPlayerController();
    if (!Localcontroller) return;
    C_CSPlayerPawn* LocalPlayer =
        Localcontroller->m_hPawn().Get<C_CSPlayerPawn>();
    if (!LocalPlayer || LocalPlayer->m_iHealth() == 0 || LocalPlayer == nullptr) return;

    auto camera_service = LocalPlayer->m_pCameraServices();
    if (!camera_service || camera_service == nullptr ||
        camera_service->m_hActivePostProcessingVolumes()
                .Get<C_PostProcessingVolume>() == NULL ||
        !camera_service->m_hActivePostProcessingVolumes()
             .Get<C_PostProcessingVolume>())
        return;


    

   if (bNightmode) {
        camera_service->m_hActivePostProcessingVolumes()
            .Get<C_PostProcessingVolume>()
            ->m_bExposureControl() = 1;
        camera_service->m_hActivePostProcessingVolumes()
            .Get<C_PostProcessingVolume>()
            ->m_flMinExposure() = max;
        camera_service->m_hActivePostProcessingVolumes()
            .Get<C_PostProcessingVolume>()
            ->m_flMaxExposure() = max;
   }
   else {
        camera_service->m_hActivePostProcessingVolumes()
            .Get<C_PostProcessingVolume>()
            ->m_bExposureControl() = 0; 
        camera_service->m_hActivePostProcessingVolumes()
            .Get<C_PostProcessingVolume>()
            ->m_flMinExposure() = 0.f;       
        camera_service->m_hActivePostProcessingVolumes()
            .Get<C_PostProcessingVolume>()
            ->m_flMaxExposure() = 0.f;
   }
}
