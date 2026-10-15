#include "../../memory/memory.hpp"
#include "ccsgoinput.hpp"
CUserCmd* CCSGOInput::GetUserCmd() {
        auto* controller = memory::fnGetLocalPlayerController ? memory::fnGetLocalPlayerController(0) : nullptr;
        if (!controller) return nullptr;
        using get_t = void* (__fastcall*)(void*);
        static auto fn = CModule(CLIENT_DLL).FindPattern(SDK_SIG("41 56 41 57 48 83 EC ? 48 8D 54 24")).Get<get_t>();
        auto* manager = fn ? static_cast<uint8_t*>(fn(controller)) : nullptr;
        if (!manager) return nullptr;
        const int sequence = *reinterpret_cast<int*>(manager + 0x5910);
        if (sequence <= 0) return nullptr;
        auto* command = reinterpret_cast<CUserCmd*>(manager) + sequence % MULTIPLAYER_BACKUP;
        return command->sequence == sequence ? command : nullptr;
    }
