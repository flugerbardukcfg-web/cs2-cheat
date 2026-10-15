#pragma once
#include <stb/stb.hh>
#include <cstring>
#include "memory.hpp"
#include "../source2-sdk/classes/cusercmd.h"
namespace CRC {
inline bool UpdateMoveCRC(CUserCmd* user_cmd) {
	if ( !user_cmd )
		return false;
	auto* base = user_cmd->pBase.pBase;
	if ( !base )
		return false;

	using string_copy_t = void( __fastcall* )( void*, const void*, int );
	using serialize_crc_t = void( __fastcall* )( void*, void*, void* );
    static auto string_copy = CModule(CLIENT_DLL).FindPattern(SDK_SIG("48 89 5C 24 10 48 89 6C 24 18 48 89 74 24 20 41 56 48 83 EC 20 48 BE FF FF FF FF FF FF FF 7F 49 8B D8 48 8B EA 4C 8B F1 4C 3B C6 0F 87 ? ? ? ?")).Get<string_copy_t>();
    static auto serialize_crc = CModule(CLIENT_DLL).FindPattern(SDK_SIG("48 89 5C 24 20 55 56 57 48 83 EC 30 49 8B C0 48 8B FA 48 8B F1 48 8B 09 F6 C1 03 0F 85 ? ? ? ? 48 85 C0 74 ? 48 8B 6A 10 48 83 7A 18 10 72 ?")).Get<serialize_crc_t>();
    if (!g_pHeapMemAlloc) return false;
	if ( !string_copy || !serialize_crc )
		return false;

	std::uint8_t bytes[ 64 ]{};
	auto* out = bytes;
 if(base->pInButtonState != nullptr) {
  const auto& buttons=*base->pInButtonState;
  const std::uint64_t states[]={buttons.State1,buttons.State2,buttons.State3};
  std::uint8_t encoded[33]{};auto* cursor=encoded;
  for(int i=0;i<3;++i) {
   std::uint64_t value=states[i];if(!value) continue;
   *cursor++=static_cast<std::uint8_t>(0x08+i*8);
   do {
    std::uint8_t byte=static_cast<std::uint8_t>(value&0x7F);value>>=7;
    *cursor++=value?static_cast<std::uint8_t>(byte|0x80):byte;
   } while(value);
  }
  const auto size=static_cast<std::uint8_t>(cursor-encoded);
  if(size){*out++=0x1a;*out++=size;std::memcpy(out,encoded,size);out+=size;}
 }
	if ( base->pViewangles != nullptr ) {
		const auto& angles = base->pViewangles->angValue;
		const float values[] = { angles.x, angles.y, angles.z };
		std::uint8_t size = 0;
		for ( const float value : values )
			if ( value != 0.f ) size += 5;
		if ( size ) {
			*out++ = 0x22;
			*out++ = size;
			for ( int i = 0; i < 3; ++i ) {
				if ( values[ i ] == 0.f ) continue;
				*out++ = static_cast<std::uint8_t>( 0x0d + i * 8 );
				std::memcpy( out, &values[ i ], 4 );
				out += 4;
			}
		}
	}

	auto* raw_base = reinterpret_cast<std::uint8_t*>( base );
	auto arena_bits = *reinterpret_cast<std::uintptr_t*>( raw_base + 0x08 );
	auto arena = arena_bits & ~std::uintptr_t{ 3 };
	if ( arena_bits & 1 ) {
		if ( !arena ) return false;
		arena = *reinterpret_cast<std::uintptr_t*>( arena );
	}
	alignas( 8 ) std::uint8_t message[ 0x20 ]{};
	string_copy( message, bytes, static_cast<int>( out - bytes ) );
	*reinterpret_cast<std::uint32_t*>( raw_base + 0x10 ) |= 1u;
	serialize_crc( raw_base + 0x30, message, reinterpret_cast<void*>( arena ) );
	 
	 
	if ( *reinterpret_cast<const std::size_t*>( message + 0x18 ) > 15 ) {
		const auto allocation = *reinterpret_cast<void* const*>( message );
		if ( allocation && g_pHeapMemAlloc )
			g_pHeapMemAlloc->Free( allocation );
	}
	return true;
}
}
