// Copyright (c) luo.dangxiao. All rights reserved.
// Licensed under the MIT License.

using System.Reflection;
using System.Runtime.InteropServices;

namespace luo.dangxiao.printer;

/// <summary>
/// Registers custom native library resolution for Seaory SDK DLLs.
/// Windows P/Invoke cannot locate native DLLs in subdirectories, so this resolver
/// maps Seaory library names to their correct output directory paths.
/// </summary>
public static class NativeLibraryResolver
{
    private static int _registered;

    /// <summary>
    /// Registers the DllImportResolver for Seaory SDK native libraries.
    /// Must be called before any Seaory P/Invoke methods are invoked.
    /// Safe to call multiple times — subsequent calls are no-ops.
    /// </summary>
    public static void Register()
    {
        if (Interlocked.Exchange(ref _registered, 1) != 0)
        {
            return;
        }

        var targetAssembly = typeof(NativeLibraryResolver).Assembly;
        NativeLibrary.SetDllImportResolver(targetAssembly, ResolveDllImport);
    }

    private static IntPtr ResolveDllImport(string libraryName, Assembly assembly, DllImportSearchPath? searchPath)
    {
        // Only resolve Seaory-family native libraries; fall back to default for everything else.
        if (!IsSeaoryLibrary(libraryName))
        {
            return IntPtr.Zero;
        }

        var subFolder = GetNativeLibSubFolder();
        if (subFolder is null)
        {
            return IntPtr.Zero;
        }

        var fullPath = Path.Combine(AppContext.BaseDirectory, subFolder, libraryName);
        return NativeLibrary.Load(fullPath, assembly, searchPath);
    }

    private static bool IsSeaoryLibrary(string libraryName)
    {
        return libraryName is "SeaorySDK.dll"
            or "libSeaorySDK.so"
            or "dcrf32.dll"
            or "libdcrf32.so"
            or "UhfReader_API.dll";
    }

    private static string? GetNativeLibSubFolder()
    {
        if (RuntimeInformation.IsOSPlatform(OSPlatform.Windows))
        {
            return RuntimeInformation.ProcessArchitecture switch
            {
                Architecture.X64 => "Seaory\\libs\\win-x64",
                Architecture.X86 => "Seaory\\libs\\win-x86",
                _ => null,
            };
        }

        if (RuntimeInformation.IsOSPlatform(OSPlatform.Linux))
        {
            return "Seaory/libs/linux-x86-64";
        }

        return null;
    }
}
