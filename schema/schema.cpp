#include <unordered_map>
#include <mutex>

#include "schema.hpp"
#include "current_schema.hpp"

#include "../interfaces/interfaces.hpp"

using SchemaKeyValueMap_t = std::unordered_map<uint32_t, int32_t>;
using SchemaTableMap_t = std::unordered_map<uint32_t, SchemaKeyValueMap_t>;

static bool InitSchemaFieldsForClass(SchemaTableMap_t& tableMap,
                                     const char* className, uint32_t classKey) {
    if (!interfaces::pSchemaSystem) return false;
    CSchemaSystemTypeScope* pType =
        interfaces::pSchemaSystem->FindTypeScopeForModule(CLIENT_DLL);
    if (!pType) return false;

    SchemaClassInfoData_t* pClassInfo = pType->FindDeclaredClass(className);
    if (!pClassInfo) {
        tableMap.emplace(classKey, SchemaKeyValueMap_t{});

        LOG("InitSchemaFieldsForClass(): '%s' was not found!\n", className);
        return false;
    }

    short fieldsSize = pClassInfo->GetFieldsSize();
    SchemaClassFieldData_t* pFields = pClassInfo->GetFields();

    if (fieldsSize < 0 || fieldsSize > 8192 || (fieldsSize && !pFields)) return false;
    auto& keyValueMap = tableMap[classKey];
    keyValueMap.reserve(fieldsSize);

    auto collect = [&](auto&& self, SchemaClassInfoData_t* info, int32_t baseOffset, int depth) -> void {
        if (!info || depth > 16) return;
        const int count = info->GetFieldsSize();
        auto* fields = info->GetFields();
        if (count < 0 || count > 8192 || (count && !fields)) return;
        const int baseCount = info->GetBaseCount();
        auto* bases = info->GetBases();
        if (bases && baseCount <= 16)
            for (int i = 0; i < baseCount; ++i) self(self, bases[i].info, baseOffset + bases[i].offset, depth + 1);
        for (int i = 0; i < count; ++i)
            if (fields[i].m_name) keyValueMap[hash_32_fnv1a_const(fields[i].m_name)] = baseOffset + fields[i].m_offset;
    };
    collect(collect, pClassInfo, 0, 0);

    LOG("schemaTableMap[%s] has %llu fields.\n", className, keyValueMap.size());
    return true;
}

int32_t schema::GetOffset(const char* className, uint32_t classKey,
                          const char* memberName, uint32_t memberKey) {
    static SchemaTableMap_t schemaTableMap;
    static std::recursive_mutex mutex;
    std::lock_guard<std::recursive_mutex> guard(mutex);
    const auto& tableMapIt = schemaTableMap.find(classKey);
    if (tableMapIt == schemaTableMap.cend()) {
        if (InitSchemaFieldsForClass(schemaTableMap, className, classKey))
            return GetOffset(className, classKey, memberName, memberKey);

        const auto currentOffset = schema::FindCurrentOffset(classKey, memberKey);
        return currentOffset >= 0 ? currentOffset : 0;
    }

    const SchemaKeyValueMap_t& tableMap = tableMapIt->second;
    if (tableMap.find(memberKey) == tableMap.cend()) {
        LOG("schema::GetOffset(): '%s' was not found in '%s'!\n", memberName,
            className);
        const auto currentOffset = schema::FindCurrentOffset(classKey, memberKey);
        return currentOffset >= 0 ? currentOffset : 0;
    }

    return tableMap.at(memberKey);
}
