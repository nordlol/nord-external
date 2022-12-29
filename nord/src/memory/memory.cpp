#include "memory.hpp"

#include <TlHelp32.h>

#include "../utils/logger/logger.hpp"

#define CALC_RECT

namespace nord
{
    bool memory::get_client_rect( HWND handle, screen_t* screen )
    {
        POINT point{};
        RECT rect{};

        if ( ClientToScreen( handle, &point ) && GetClientRect( handle, &rect ) )
        {
#ifndef CALC_RECT
            *screen = { point.x, point.y, 500, 500 };
#else
            *screen = { point.x, point.y, rect.right - rect.left, rect.bottom - rect.top };
#endif  // !USE_CSHARP

            return true;
        }

        return false;
    }

    bool memory::start()
    {
        if ( !( handle = get_process() ) )
            return false;

        log_mgr.log_debug( "memory", "Retrieved process handle 0x%x\n", handle );

        // module name is the same as the process name
        const auto modopt = get_module( handle, name );

        if ( !modopt.has_value() )
            return false;

        module = modopt.value();

        log_mgr.log_debug( "memory", "Retrieved process module --> base: 0x%x, size: %d\n", module.base, module.size );

        return true;
    }

    HANDLE memory::get_process()
    {
        if ( HANDLE snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, NULL ) )
        {
            PROCESSENTRY32 entry;
            entry.dwSize = sizeof( PROCESSENTRY32 );

            if ( Process32First( snapshot, &entry ) )
            {
                do
                {
                    if ( !std::wcscmp( entry.szExeFile, name ) )
                    {
                        if ( HANDLE proc_handle = OpenProcess( PROCESS_ALL_ACCESS, false, entry.th32ProcessID ) )
                        {
                            BOOL has_debugger = FALSE;
                            CheckRemoteDebuggerPresent( proc_handle, &has_debugger );

                            if ( has_debugger )
                                continue;

                            CloseHandle( snapshot );
                            return proc_handle;
                        }
                    }

                } while ( Process32Next( snapshot, &entry ) );
            }

            CloseHandle( snapshot );
        }
        return nullptr;
    }

    std::optional< module_t > memory::get_module( HANDLE handle, const wchar_t* module_name )
    {
        if ( HANDLE snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, GetProcessId( handle ) ) )
        {
            MODULEENTRY32 entry;
            entry.dwSize = sizeof( MODULEENTRY32 );

            if ( Module32First( snapshot, &entry ) )
            {
                do
                {
                    if ( !std::wcscmp( entry.szModule, module_name ) )
                    {
                        CloseHandle( snapshot );
                        return module_t{ reinterpret_cast< std::uintptr_t >( entry.modBaseAddr ),
                                         static_cast< std::uintptr_t >( entry.modBaseSize ) };
                    }

                } while ( Module32Next( snapshot, &entry ) );
            }

            CloseHandle( snapshot );
        }
        return std::nullopt;
    }

    std::uintptr_t memory::find_pattern( const char* const pattern, std::uintptr_t size )
    {
        // if size isn't specified, scan entire process
        if ( !size )
            size = module.size;

        std::vector< std::uint8_t > buffer;
        buffer.resize( size );

        if ( ReadProcessMemory( handle, reinterpret_cast< LPCVOID >( module.base ), buffer.data(), size, nullptr ) )
        {
            for ( std::size_t offset = 0; offset < buffer.size(); offset++ )
            {
                if ( check_pattern( buffer.data(), pattern ) )
                    return module.base + offset;
            }
        }

        return 0;
    }

    bool memory::check_pattern( const std::uint8_t* data, const char* pattern )
    {
        for ( ; *pattern; *pattern != ' ' ? ++data : data, ++pattern )
        {
            if ( *pattern == ' ' || *pattern == '?' )
                continue;
            if ( *data != get_byte( pattern ) )
                return false;
            ++pattern;
        }
        return true;
    }

    bool memory::destroy()
    {
        return CloseHandle( handle );
    }
}  // namespace nord