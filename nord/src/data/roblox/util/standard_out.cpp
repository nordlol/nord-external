#include "standard_out.hpp"

#include <format>

namespace nord::rbx
{
    std::shared_ptr< standard_out > standard_out::get()
    {
        static std::shared_ptr< standard_out > sout( new standard_out() );
        return sout;
    }

    //// payload
    // struct payload_t
    //{
    //    unsigned int type;
    //    const char *message;

    //    std::uintptr_t address;
    //};
    // typedef unsigned int( __fastcall *_print )( unsigned int, const char * );
    // std::uintptr_t __stdcall RemoteThread( payload_t *data )
    //{
    //    _print print = ( _print )data->address;
    //    print( data->type, data->message );
    //    return EXIT_SUCCESS;
    //}

    // void call( payload_t data, HANDLE handle )
    //{
    //    LPVOID pRemoteThread =
    //        VirtualAllocEx( handle, NULL, sizeof( payload_t ), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE );

    //    WriteProcessMemory( handle, pRemoteThread, ( LPVOID )RemoteThread, sizeof( payload_t ), nullptr );

    //    payload_t *pData = ( payload_t * )VirtualAllocEx( handle, NULL, sizeof( payload_t ), MEM_COMMIT, PAGE_READWRITE );

    //    WriteProcessMemory( handle, pData, &data, sizeof( payload_t ), NULL );

    //    HANDLE hThread = CreateRemoteThread( handle, 0, 0, ( LPTHREAD_START_ROUTINE )pRemoteThread, pData, 0, 0 );

    //    CloseHandle( hThread );

    //    VirtualFreeEx( handle, pRemoteThread, sizeof( payload_t ), MEM_RELEASE );
    //}

    void standard_out::print( message_type type, const char* message )
    {
    }

}  // namespace nord::rbx