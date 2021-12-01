workspace "MinecraftYoutube"
    architecture "x64"

    configurations {
        "Debug",
        "Release",
        "Dist"
    }

    startproject "MinecraftYoutube"

-- This is a helper variable, to concatenate the sys-arch
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "MinecraftYoutube"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir("bin/" .. outputdir .. "/{prj.name}")
    objdir("bin-int/" .. outputdir .. "/{prj.name}")

    files {
        "MinecraftYoutube/src/**.cpp",
        "MinecraftYoutube/include/**.h",
        "MinecraftYoutube/vendor/GLFW/include/GLFW/glfw3.h",
        "MinecraftYoutube/vendor/GLFW/include/GLFW/glfw3native.h",
        "MinecraftYoutube/vendor/GLFW/src/glfw_config.h",
        "MinecraftYoutube/vendor/GLFW/src/context.c",
        "MinecraftYoutube/vendor/GLFW/src/init.c",
        "MinecraftYoutube/vendor/GLFW/src/input.c",
        "MinecraftYoutube/vendor/GLFW/src/monitor.c",
        "MinecraftYoutube/vendor/GLFW/src/vulkan.c",
        "MinecraftYoutube/vendor/GLFW/src/window.c",
        "MinecraftYoutube/vendor/GLFW/src/platform.c",
        "MinecraftYoutube/vendor/GLFW/src/null**.c",
        "MinecraftYoutube/vendor/glad/include/glad/glad.h",
        "MinecraftYoutube/vendor/glad/include/glad/KHR/khrplatform.h",
		"MinecraftYoutube/vendor/glad/src/glad.c"
    }

    includedirs {
        "MinecraftYoutube/include",
        "MinecraftYoutube/vendor/GLFW/include",
        "MinecraftYoutube/vendor/glad/include"
    }

    filter "system:windows"
        buildoptions { "-lgdi32" }
        systemversion "latest"

        files {
            "MinecraftYoutube/vendor/GLFW/src/win32_init.c",
            "MinecraftYoutube/vendor/GLFW/src/win32_joystick.c",
            "MinecraftYoutube/vendor/GLFW/src/win32_monitor.c",
            "MinecraftYoutube/vendor/GLFW/src/win32_time.c",
            "MinecraftYoutube/vendor/GLFW/src/win32_thread.c",
            "MinecraftYoutube/vendor/GLFW/src/win32_window.c",
            "MinecraftYoutube/vendor/GLFW/src/wgl_context.c",
            "MinecraftYoutube/vendor/GLFW/src/egl_context.c",
            "MinecraftYoutube/vendor/GLFW/src/osmesa_context.c",
            "MinecraftYoutube/vendor/GLFW/src/win32_module.c",
            "MinecraftYoutube/vendor/GLFW/src/win32_platform.h"
        }

        defines  {
            "_GLFW_WIN32",
            "_CRT_SECURE_NO_WARNINGS"
        }

    filter { "configurations:Debug" }
        buildoptions "/MTd"
        runtime "Debug"
        symbols "on"

    filter { "configurations:Release" }
        buildoptions "/MT"
        runtime "Release"
        optimize "on"

