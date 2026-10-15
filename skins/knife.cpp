#include "knife.h"
#include "../../sdk/interfaces/interfaces.hpp"
#include <unordered_map>
#include <unordered_set>
#include <cstdio>
namespace {
std::uint32_t schema_get_offset(const char* cls, const char* field) {
    return schema::GetOffset(cls, hash_32_fnv1a_const(cls), field, hash_32_fnv1a_const(field));
}
template<class T> bool read_at(const void* base, std::uint32_t offset, T& result) {
    __try {
        if (!base || !offset) return false;
        result = *reinterpret_cast<const T*>(static_cast<const std::byte*>(base) + offset);
        return true;
    } __except (EXCEPTION_EXECUTE_HANDLER) { return false; }
}
template<class T> bool read_direct(const T* address, T& result) {
    __try { if (!address) return false; result = *address; return true; }
    __except (EXCEPTION_EXECUTE_HANDLER) { return false; }
}
template<class T> bool write_at(void* base, std::uint32_t offset, T value) {
    __try {
        if (!base || !offset) return false;
        *reinterpret_cast<T*>(static_cast<std::byte*>(base) + offset) = value;
        return true;
    } __except (EXCEPTION_EXECUTE_HANDLER) { return false; }
}
std::uint32_t knife_subclass(std::uint16_t definition) {
    switch (definition) {
    case 500: return 3933374535u;
    case 503: return 3787235507u;
    case 505: return 4046390180u;
    case 506: return 2047704618u;
    case 507: return 1731408398u;
    case 508: return 1638561588u;
    case 509: return 2282479884u;
    case 512: return 3412259219u;
    case 514: return 2511498851u;
    case 515: return 1353709123u;
    case 516: return 4269888884u;
    case 517: return 1105782941u;
    case 518: return 275962944u;
    case 519: return 1338637359u;
    case 520: return 3230445913u;
    case 521: return 3206681373u;
    case 522: return 2595277776u;
    case 523: return 4029975521u;
    case 525: return 365028728u;
    case 526: return 3845286452u;
    default: return 0;
    }
}
bool knife_model_name(std::uint16_t definition, char (&model)[260]) {
    for (const auto& entry : knife_changer::knifes_data) if (entry.def_index == definition) {
        std::snprintf(model, sizeof(model), "%s", entry.model_path.c_str()); return true;
    }
    const char* path = definition == 42 ? "weapons/models/knife/knife_default_ct/weapon_knife_default_ct.vmdl" :
        (definition == 41 || definition == 59) ? "weapons/models/knife/knife_default_t/weapon_knife_default_t.vmdl" : nullptr;
    if (!path) return false;
    std::snprintf(model, sizeof(model), "%s", path); return true;
}
using set_model_fn = void(__fastcall*)(void*,const char*);
using update_subclass_fn = void(__fastcall*)(void*);
using precache_fn = void*(__fastcall*)(void*,void*,const char*);
using insert_string_fn = const char*(__fastcall*)(void*,int,const char*,int,bool);
using purge_string_fn = void(__fastcall*)(void*,int);
bool invoke_precache(precache_fn precache, insert_string_fn insert, purge_string_fn purge,
    void* system, const char* model) {
    struct buffer_t { int length; std::uint32_t flags; std::uint64_t data; };
    buffer_t buffer{0,0xC0000008u,0};
    __try {
        insert(&buffer,0,model,-1,false);
        precache(system,&buffer,"");
        purge(&buffer,0);
        return true;
    } __except (EXCEPTION_EXECUTE_HANDLER) { return false; }
}
void precache_model(const char* model) {
    static const auto module = GetModuleHandleA("resourcesystem.dll");
    static const auto factory = module ? reinterpret_cast<void*(*)(const char*,int*)>(GetProcAddress(module,"CreateInterface")) : nullptr;
    static void* system = factory ? factory("ResourceSystem013",nullptr) : nullptr;
    static auto precache = reinterpret_cast<precache_fn>(CModule("resourcesystem.dll").FindPattern(SDK_SIG("40 53 55 57 48 81 EC ? ? ? ? 48 8B 01 49 8B E8 48 8B FA")).Get<void*>());
    static const auto tier = GetModuleHandleA("tier0.dll");
    static auto insert = tier ? reinterpret_cast<insert_string_fn>(GetProcAddress(tier,"?Insert@CBufferString@@QEAAPEBDHPEBDH_N@Z")) : nullptr;
    static auto purge = tier ? reinterpret_cast<purge_string_fn>(GetProcAddress(tier,"?Purge@CBufferString@@QEAAXH@Z")) : nullptr;
    static std::unordered_set<std::string> loaded;
    if (system && precache && insert && purge && !loaded.contains(model) &&
        invoke_precache(precache,insert,purge,system,model)) loaded.emplace(model);
}
bool invoke_set_model(set_model_fn set_model, void* weapon, const char* model) {
    __try { set_model(weapon,model); return true; }
    __except (EXCEPTION_EXECUTE_HANDLER) { return false; }
}
bool invoke_subclass_update(update_subclass_fn update, void* weapon) {
    __try { update(weapon); return true; }
    __except (EXCEPTION_EXECUTE_HANDLER) { return false; }
}
bool change_knife_model(void* weapon, void* item, std::uint16_t definition,
    std::uint32_t subclass_offset, std::uint32_t subclass) {
    if (!weapon || !item || !subclass_offset) return false;
    char model[260]{};
    if (!knife_model_name(definition,model)) return false;
    static auto set_model = reinterpret_cast<set_model_fn>(CModule(CLIENT_DLL).FindPattern(SDK_SIG("40 53 48 83 EC ? 48 8B D9 4C 8B C2 48 8B 0D ? ? ? ? 48 8D 54 24")).Get<void*>());
    static auto update = reinterpret_cast<update_subclass_fn>(CModule(CLIENT_DLL).FindPattern(SDK_SIG("4C 8B DC 53 48 81 EC ? ? ? ? 48 8B 41")).Get<void*>());
    if (!set_model || !update) return false;
    precache_model(model);
    static const auto definition_offset = schema_get_offset(
        "C_EconItemView","m_iItemDefinitionIndex");
    std::uint16_t old_definition = 0;
    std::uint32_t old_subclass = 0;
    if (!read_at(item,definition_offset,old_definition) ||
        !read_at(weapon,subclass_offset,old_subclass)) return false;
    if (!write_at(item,definition_offset,definition)) return false;
    if (invoke_set_model(set_model,weapon,model) &&
        write_at(weapon,subclass_offset,subclass) &&
        invoke_subclass_update(update,weapon)) return true;
    write_at(item,definition_offset,old_definition);
    write_at(weapon,subclass_offset,old_subclass);
    char old_model[260]{};
    if (knife_model_name(old_definition,old_model))
        invoke_set_model(set_model,weapon,old_model);
    invoke_subclass_update(update,weapon);
    return false;
}
using rebuild_graph_fn = void(__fastcall*)(void*,std::uint8_t);
bool invoke_graph_rebuild(void* hud, rebuild_graph_fn rebuild,
    std::uint32_t body_offset, std::uint32_t controller_offset, std::uint32_t graph_offset) {
    __try {
        auto* body = *reinterpret_cast<std::byte**>(static_cast<std::byte*>(hud) + body_offset);
        if (!body) return false;
        auto* controller = body + controller_offset;
        if (!*reinterpret_cast<void**>(controller + graph_offset)) return false;
        rebuild(controller,2);
        return true;
    } __except (EXCEPTION_EXECUTE_HANDLER) { return false; }
}
bool rebuild_hud_anim_graph(void* hud) {
    static auto rebuild = reinterpret_cast<rebuild_graph_fn>(CModule(CLIENT_DLL).FindPattern(SDK_SIG("40 55 56 48 83 EC 28 4C 89 74 24 58 48 8B F1 80 FA FF 75 04 0F B6 51 18")).Get<void*>());
    static const auto body_offset = schema_get_offset("C_BaseEntity","m_CBodyComponent");
    static const auto controller_offset = schema_get_offset(
        "CBodyComponentBaseAnimGraph","m_animationController");
    static const auto graph_offset = schema_get_offset(
        "CBaseAnimGraphController","m_pGraphInstanceAG2");
    if (!hud || !rebuild || !body_offset || !controller_offset || !graph_offset) return false;
    return invoke_graph_rebuild(hud,rebuild,body_offset,controller_offset,graph_offset);
}

struct State {
    uint16_t originalDefinition{}, appliedDefinition{};
    uint32_t originalSubclass{};
    CHandle hud;
    ULONGLONG graphDue{};
    unsigned graphTries{};
};
std::unordered_map<uint32_t, State> states;
bool is_knife(uint16_t id) { return id == 41 || id == 42 || id == 59 || knife_subclass(id) != 0; }
C_BaseEntity* find_hud(CGameEntitySystem* system, C_CSWeaponBase* weapon) {
    for (int i=1, highest=system->GetHighestEntityIndex(); i<=highest; ++i) {
        auto* candidate = system->GetBaseEntity(i);
        auto* info = candidate ? candidate->Schema_DynamicBinding() : nullptr;
        if (info && info->GetName() && !std::strcmp(info->GetName(), "C_CS2HudModelWeapon") && candidate->m_hOwnerEntity() == weapon->GetRefEHandle()) return candidate;
    }
    return nullptr;
}
void update_hud(C_BaseEntity* hud, State& state) {
    if (!hud) return;
    state.hud = hud->GetRefEHandle();
    state.graphDue = GetTickCount64() + 250;
    state.graphTries = 0;
}
}
void knife_changer::Run() {
    if (!interfaces::pEngine || !interfaces::pEngine->IsInGame()) { states.clear(); return; }
    const int selection = selected;
    const bool active = enable && selection >= 1 && selection <= static_cast<int>(knifes_data.size());
    auto* system = CGameEntitySystem::GetInstance();
    if (!system) return;
    for (auto it=states.begin(); it!=states.end();) {
        CHandle handle; handle.m_Index=it->first;
        auto* weapon=handle.Get<C_CSWeaponBase>();
        if (!weapon) { it=states.erase(it); continue; }
        if (!active) {
            auto* item=weapon->m_AttributeManager()->m_Item();
            if (change_knife_model(weapon,item,it->second.originalDefinition,schema_get_offset("C_BaseEntity","m_nSubclassID"),it->second.originalSubclass)) {
                auto* hud=find_hud(system,weapon);
                rebuild_hud_anim_graph(hud);
                it=states.erase(it); continue;
            }
        }
        ++it;
    }
    if (!active) return;
    auto* controller=CGameEntitySystem::GetLocalPlayerController();
    auto* pawn=controller ? controller->m_hPawn().Get<C_CSPlayerPawn>() : nullptr;
    if (!pawn || pawn->m_iHealth() <= 0) return;
    auto* weapon=pawn->GetWeaponActive();
    auto* manager=weapon ? weapon->m_AttributeManager() : nullptr;
    auto* item=manager ? manager->m_Item() : nullptr;
    if (!item || !is_knife(item->m_iItemDefinitionIndex())) return;
    const auto definition=static_cast<uint16_t>(knifes_data[selection-1].def_index);
    const auto subclass=knife_subclass(definition);
    auto [it,added]=states.try_emplace(weapon->GetRefEHandle().m_Index);
    auto& state=it->second;
    if (added) { state.originalDefinition=item->m_iItemDefinitionIndex(); state.originalSubclass=weapon->m_nSubclassID(); }
    auto* hud=find_hud(system,weapon);
    if (state.appliedDefinition != definition || item->m_iItemDefinitionIndex() != definition || weapon->m_nSubclassID() != subclass) {
        if (!change_knife_model(weapon,item,definition,schema_get_offset("C_BaseEntity","m_nSubclassID"),subclass)) return;
        state.appliedDefinition=definition; state.hud=CHandle{};
        state.graphDue=0; state.graphTries=0;
        update_hud(hud,state);
    } else if (hud && !(hud->GetRefEHandle() == state.hud)) update_hud(hud,state);
    if (hud && state.graphDue && GetTickCount64() >= state.graphDue) {
        if (rebuild_hud_anim_graph(hud) || ++state.graphTries >= 8) state.graphDue=0;
        else state.graphDue=GetTickCount64()+250;
    }
}
