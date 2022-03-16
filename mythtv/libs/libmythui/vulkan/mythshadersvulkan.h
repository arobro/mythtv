#ifndef MYTHSHADERSVULKAN_H
#define MYTHSHADERSVULKAN_H

// Qt
#include <QString>

// MythTV
#include "libmythui/vulkan/mythcombobuffervulkan.h"
#include "libmythui/vulkan/mythshadervulkan.h"

// GLSL 4.50 shaders for Vulkan.
// N.B. These cannot, as written, be used for OpenGL as they use descriptor sets
// and push constants - which are Vulkan only features.

// Normal/release builds will always use SPIR-V bytecode for shaders.
// To update shaders you must:
//  - install libglslang (not usually available as a package)
//  - edit GLSL sources as needed
//  - set MYTHTV_GLSLANG environment variable
//  - run mythfrontend
//  - cut and paste updated SPIR-V from the logs (if compilation worked!)

#define DefaultVertex450   (VK_SHADER_STAGE_VERTEX_BIT   | (1 << 6))
#define DefaultFragment450 (VK_SHADER_STAGE_FRAGMENT_BIT | (1 << 7))

static const MythBindingMap k450ShaderBindings = {
    { DefaultVertex450,
        { VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,
        { { 0, { 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT, nullptr } } },
        { },
        { },
        { VK_SHADER_STAGE_VERTEX_BIT, 0, MYTH_PUSHBUFFER_SIZE } }
    },
    { DefaultFragment450,
        { VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,
        { { 1, { 0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr } } },
        { },
        { },
        { } }
    }
};

static const MythShaderMap k450DefaultShaders = {
{
DefaultVertex450,
{
"#version 450\n"
"#extension GL_ARB_separate_shader_objects : enable\n"
"layout(set = 0, binding = 0) uniform Projection { mat4 projection; } proj;\n"
"layout(push_constant) uniform ComboBuffer {\n"
"    mat4 transform;\n"
"    vec4 positions;\n"
"    vec4 texcoords;\n"
"    vec4 color;\n"
"} cb;\n"
"layout(location = 0) out vec2 fragCoord;\n"
"layout(location = 1) out vec4 fragColor;\n"
"void main() {\n"
"    vec2 vertices[4] = { vec2(cb.positions[0], cb.positions[1]),\n"
"                         vec2(cb.positions[0], cb.positions[3]),\n"
"                         vec2(cb.positions[2], cb.positions[1]),\n"
"                         vec2(cb.positions[2], cb.positions[3]) };\n"
"    vec2 texcoord[4] = { vec2(cb.texcoords[0], cb.texcoords[1]),\n"
"                         vec2(cb.texcoords[0], cb.texcoords[3]),\n"
"                         vec2(cb.texcoords[2], cb.texcoords[1]),\n"
"                         vec2(cb.texcoords[2], cb.texcoords[3]) };\n"
"    gl_Position = proj.projection * cb.transform * vec4(vertices[gl_VertexIndex], 0.0, 1.0);\n"
"    fragCoord   = texcoord[gl_VertexIndex];\n"
"    fragColor   = cb.color;\n"
"}\n",
{
0x07230203, 0x00010300, 0x00080008, 0x0000006C,
0x00000000, 0x00020011, 0x00000001, 0x0006000B,
0x00000001, 0x4C534C47, 0x6474732E, 0x3035342E,
0x00000000, 0x0003000E, 0x00000000, 0x00000001,
0x0009000F, 0x00000000, 0x00000004, 0x6E69616D,
0x00000000, 0x00000048, 0x00000055, 0x00000063,
0x00000067, 0x00030003, 0x00000002, 0x000001C2,
0x00090004, 0x415F4C47, 0x735F4252, 0x72617065,
0x5F657461, 0x64616873, 0x6F5F7265, 0x63656A62,
0x00007374, 0x00040005, 0x00000004, 0x6E69616D,
0x00000000, 0x00050005, 0x0000000C, 0x74726576,
0x73656369, 0x00000000, 0x00050005, 0x0000000F,
0x626D6F43, 0x6675426F, 0x00726566, 0x00060006,
0x0000000F, 0x00000000, 0x6E617274, 0x726F6673,
0x0000006D, 0x00060006, 0x0000000F, 0x00000001,
0x69736F70, 0x6E6F6974, 0x00000073, 0x00060006,
0x0000000F, 0x00000002, 0x63786574, 0x64726F6F,
0x00000073, 0x00050006, 0x0000000F, 0x00000003,
0x6F6C6F63, 0x00000072, 0x00030005, 0x00000011,
0x00006263, 0x00050005, 0x0000002E, 0x63786574,
0x64726F6F, 0x00000000, 0x00060005, 0x00000046,
0x505F6C67, 0x65567265, 0x78657472, 0x00000000,
0x00060006, 0x00000046, 0x00000000, 0x505F6C67,
0x7469736F, 0x006E6F69, 0x00070006, 0x00000046,
0x00000001, 0x505F6C67, 0x746E696F, 0x657A6953,
0x00000000, 0x00070006, 0x00000046, 0x00000002,
0x435F6C67, 0x4470696C, 0x61747369, 0x0065636E,
0x00070006, 0x00000046, 0x00000003, 0x435F6C67,
0x446C6C75, 0x61747369, 0x0065636E, 0x00030005,
0x00000048, 0x00000000, 0x00050005, 0x0000004A,
0x6A6F7250, 0x69746365, 0x00006E6F, 0x00060006,
0x0000004A, 0x00000000, 0x6A6F7270, 0x69746365,
0x00006E6F, 0x00040005, 0x0000004C, 0x6A6F7270,
0x00000000, 0x00060005, 0x00000055, 0x565F6C67,
0x65747265, 0x646E4978, 0x00007865, 0x00050005,
0x00000063, 0x67617266, 0x726F6F43, 0x00000064,
0x00050005, 0x00000067, 0x67617266, 0x6F6C6F43,
0x00000072, 0x00040048, 0x0000000F, 0x00000000,
0x00000005, 0x00050048, 0x0000000F, 0x00000000,
0x00000023, 0x00000000, 0x00050048, 0x0000000F,
0x00000000, 0x00000007, 0x00000010, 0x00050048,
0x0000000F, 0x00000001, 0x00000023, 0x00000040,
0x00050048, 0x0000000F, 0x00000002, 0x00000023,
0x00000050, 0x00050048, 0x0000000F, 0x00000003,
0x00000023, 0x00000060, 0x00030047, 0x0000000F,
0x00000002, 0x00050048, 0x00000046, 0x00000000,
0x0000000B, 0x00000000, 0x00050048, 0x00000046,
0x00000001, 0x0000000B, 0x00000001, 0x00050048,
0x00000046, 0x00000002, 0x0000000B, 0x00000003,
0x00050048, 0x00000046, 0x00000003, 0x0000000B,
0x00000004, 0x00030047, 0x00000046, 0x00000002,
0x00040048, 0x0000004A, 0x00000000, 0x00000005,
0x00050048, 0x0000004A, 0x00000000, 0x00000023,
0x00000000, 0x00050048, 0x0000004A, 0x00000000,
0x00000007, 0x00000010, 0x00030047, 0x0000004A,
0x00000002, 0x00040047, 0x0000004C, 0x00000022,
0x00000000, 0x00040047, 0x0000004C, 0x00000021,
0x00000000, 0x00040047, 0x00000055, 0x0000000B,
0x0000002A, 0x00040047, 0x00000063, 0x0000001E,
0x00000000, 0x00040047, 0x00000067, 0x0000001E,
0x00000001, 0x00020013, 0x00000002, 0x00030021,
0x00000003, 0x00000002, 0x00030016, 0x00000006,
0x00000020, 0x00040017, 0x00000007, 0x00000006,
0x00000002, 0x00040015, 0x00000008, 0x00000020,
0x00000000, 0x0004002B, 0x00000008, 0x00000009,
0x00000004, 0x0004001C, 0x0000000A, 0x00000007,
0x00000009, 0x00040020, 0x0000000B, 0x00000007,
0x0000000A, 0x00040017, 0x0000000D, 0x00000006,
0x00000004, 0x00040018, 0x0000000E, 0x0000000D,
0x00000004, 0x0006001E, 0x0000000F, 0x0000000E,
0x0000000D, 0x0000000D, 0x0000000D, 0x00040020,
0x00000010, 0x00000009, 0x0000000F, 0x0004003B,
0x00000010, 0x00000011, 0x00000009, 0x00040015,
0x00000012, 0x00000020, 0x00000001, 0x0004002B,
0x00000012, 0x00000013, 0x00000001, 0x0004002B,
0x00000008, 0x00000014, 0x00000000, 0x00040020,
0x00000015, 0x00000009, 0x00000006, 0x0004002B,
0x00000008, 0x00000018, 0x00000001, 0x0004002B,
0x00000008, 0x0000001E, 0x00000003, 0x0004002B,
0x00000008, 0x00000022, 0x00000002, 0x0004002B,
0x00000012, 0x0000002F, 0x00000002, 0x0004001C,
0x00000045, 0x00000006, 0x00000018, 0x0006001E,
0x00000046, 0x0000000D, 0x00000006, 0x00000045,
0x00000045, 0x00040020, 0x00000047, 0x00000003,
0x00000046, 0x0004003B, 0x00000047, 0x00000048,
0x00000003, 0x0004002B, 0x00000012, 0x00000049,
0x00000000, 0x0003001E, 0x0000004A, 0x0000000E,
0x00040020, 0x0000004B, 0x00000002, 0x0000004A,
0x0004003B, 0x0000004B, 0x0000004C, 0x00000002,
0x00040020, 0x0000004D, 0x00000002, 0x0000000E,
0x00040020, 0x00000050, 0x00000009, 0x0000000E,
0x00040020, 0x00000054, 0x00000001, 0x00000012,
0x0004003B, 0x00000054, 0x00000055, 0x00000001,
0x00040020, 0x00000057, 0x00000007, 0x00000007,
0x0004002B, 0x00000006, 0x0000005A, 0x00000000,
0x0004002B, 0x00000006, 0x0000005B, 0x3F800000,
0x00040020, 0x00000060, 0x00000003, 0x0000000D,
0x00040020, 0x00000062, 0x00000003, 0x00000007,
0x0004003B, 0x00000062, 0x00000063, 0x00000003,
0x0004003B, 0x00000060, 0x00000067, 0x00000003,
0x0004002B, 0x00000012, 0x00000068, 0x00000003,
0x00040020, 0x00000069, 0x00000009, 0x0000000D,
0x00050036, 0x00000002, 0x00000004, 0x00000000,
0x00000003, 0x000200F8, 0x00000005, 0x0004003B,
0x0000000B, 0x0000000C, 0x00000007, 0x0004003B,
0x0000000B, 0x0000002E, 0x00000007, 0x00060041,
0x00000015, 0x00000016, 0x00000011, 0x00000013,
0x00000014, 0x0004003D, 0x00000006, 0x00000017,
0x00000016, 0x00060041, 0x00000015, 0x00000019,
0x00000011, 0x00000013, 0x00000018, 0x0004003D,
0x00000006, 0x0000001A, 0x00000019, 0x00050050,
0x00000007, 0x0000001B, 0x00000017, 0x0000001A,
0x00060041, 0x00000015, 0x0000001C, 0x00000011,
0x00000013, 0x00000014, 0x0004003D, 0x00000006,
0x0000001D, 0x0000001C, 0x00060041, 0x00000015,
0x0000001F, 0x00000011, 0x00000013, 0x0000001E,
0x0004003D, 0x00000006, 0x00000020, 0x0000001F,
0x00050050, 0x00000007, 0x00000021, 0x0000001D,
0x00000020, 0x00060041, 0x00000015, 0x00000023,
0x00000011, 0x00000013, 0x00000022, 0x0004003D,
0x00000006, 0x00000024, 0x00000023, 0x00060041,
0x00000015, 0x00000025, 0x00000011, 0x00000013,
0x00000018, 0x0004003D, 0x00000006, 0x00000026,
0x00000025, 0x00050050, 0x00000007, 0x00000027,
0x00000024, 0x00000026, 0x00060041, 0x00000015,
0x00000028, 0x00000011, 0x00000013, 0x00000022,
0x0004003D, 0x00000006, 0x00000029, 0x00000028,
0x00060041, 0x00000015, 0x0000002A, 0x00000011,
0x00000013, 0x0000001E, 0x0004003D, 0x00000006,
0x0000002B, 0x0000002A, 0x00050050, 0x00000007,
0x0000002C, 0x00000029, 0x0000002B, 0x00070050,
0x0000000A, 0x0000002D, 0x0000001B, 0x00000021,
0x00000027, 0x0000002C, 0x0003003E, 0x0000000C,
0x0000002D, 0x00060041, 0x00000015, 0x00000030,
0x00000011, 0x0000002F, 0x00000014, 0x0004003D,
0x00000006, 0x00000031, 0x00000030, 0x00060041,
0x00000015, 0x00000032, 0x00000011, 0x0000002F,
0x00000018, 0x0004003D, 0x00000006, 0x00000033,
0x00000032, 0x00050050, 0x00000007, 0x00000034,
0x00000031, 0x00000033, 0x00060041, 0x00000015,
0x00000035, 0x00000011, 0x0000002F, 0x00000014,
0x0004003D, 0x00000006, 0x00000036, 0x00000035,
0x00060041, 0x00000015, 0x00000037, 0x00000011,
0x0000002F, 0x0000001E, 0x0004003D, 0x00000006,
0x00000038, 0x00000037, 0x00050050, 0x00000007,
0x00000039, 0x00000036, 0x00000038, 0x00060041,
0x00000015, 0x0000003A, 0x00000011, 0x0000002F,
0x00000022, 0x0004003D, 0x00000006, 0x0000003B,
0x0000003A, 0x00060041, 0x00000015, 0x0000003C,
0x00000011, 0x0000002F, 0x00000018, 0x0004003D,
0x00000006, 0x0000003D, 0x0000003C, 0x00050050,
0x00000007, 0x0000003E, 0x0000003B, 0x0000003D,
0x00060041, 0x00000015, 0x0000003F, 0x00000011,
0x0000002F, 0x00000022, 0x0004003D, 0x00000006,
0x00000040, 0x0000003F, 0x00060041, 0x00000015,
0x00000041, 0x00000011, 0x0000002F, 0x0000001E,
0x0004003D, 0x00000006, 0x00000042, 0x00000041,
0x00050050, 0x00000007, 0x00000043, 0x00000040,
0x00000042, 0x00070050, 0x0000000A, 0x00000044,
0x00000034, 0x00000039, 0x0000003E, 0x00000043,
0x0003003E, 0x0000002E, 0x00000044, 0x00050041,
0x0000004D, 0x0000004E, 0x0000004C, 0x00000049,
0x0004003D, 0x0000000E, 0x0000004F, 0x0000004E,
0x00050041, 0x00000050, 0x00000051, 0x00000011,
0x00000049, 0x0004003D, 0x0000000E, 0x00000052,
0x00000051, 0x00050092, 0x0000000E, 0x00000053,
0x0000004F, 0x00000052, 0x0004003D, 0x00000012,
0x00000056, 0x00000055, 0x00050041, 0x00000057,
0x00000058, 0x0000000C, 0x00000056, 0x0004003D,
0x00000007, 0x00000059, 0x00000058, 0x00050051,
0x00000006, 0x0000005C, 0x00000059, 0x00000000,
0x00050051, 0x00000006, 0x0000005D, 0x00000059,
0x00000001, 0x00070050, 0x0000000D, 0x0000005E,
0x0000005C, 0x0000005D, 0x0000005A, 0x0000005B,
0x00050091, 0x0000000D, 0x0000005F, 0x00000053,
0x0000005E, 0x00050041, 0x00000060, 0x00000061,
0x00000048, 0x00000049, 0x0003003E, 0x00000061,
0x0000005F, 0x0004003D, 0x00000012, 0x00000064,
0x00000055, 0x00050041, 0x00000057, 0x00000065,
0x0000002E, 0x00000064, 0x0004003D, 0x00000007,
0x00000066, 0x00000065, 0x0003003E, 0x00000063,
0x00000066, 0x00050041, 0x00000069, 0x0000006A,
0x00000011, 0x00000068, 0x0004003D, 0x0000000D,
0x0000006B, 0x0000006A, 0x0003003E, 0x00000067,
0x0000006B, 0x000100FD, 0x00010038
} } },
{
DefaultFragment450,
{
"#version 450\n"
"#extension GL_ARB_separate_shader_objects : enable\n"
"layout(set = 1, binding = 0) uniform sampler2D texSampler;\n"
"layout(location = 0) in  vec2 fragCoord;\n"
"layout(location = 1) in  vec4 texColor;\n"
"layout(location = 0) out vec4 fragColor;\n"
"void main() {\n"
"    fragColor = texture(texSampler, fragCoord) * texColor;\n"
"}\n",
{
0x07230203, 0x00010300, 0x00080008, 0x00000018,
0x00000000, 0x00020011, 0x00000001, 0x0006000B,
0x00000001, 0x4C534C47, 0x6474732E, 0x3035342E,
0x00000000, 0x0003000E, 0x00000000, 0x00000001,
0x0008000F, 0x00000004, 0x00000004, 0x6E69616D,
0x00000000, 0x00000009, 0x00000011, 0x00000015,
0x00030010, 0x00000004, 0x00000007, 0x00030003,
0x00000002, 0x000001C2, 0x00090004, 0x415F4C47,
0x735F4252, 0x72617065, 0x5F657461, 0x64616873,
0x6F5F7265, 0x63656A62, 0x00007374, 0x00040005,
0x00000004, 0x6E69616D, 0x00000000, 0x00050005,
0x00000009, 0x67617266, 0x6F6C6F43, 0x00000072,
0x00050005, 0x0000000D, 0x53786574, 0x6C706D61,
0x00007265, 0x00050005, 0x00000011, 0x67617266,
0x726F6F43, 0x00000064, 0x00050005, 0x00000015,
0x43786574, 0x726F6C6F, 0x00000000, 0x00040047,
0x00000009, 0x0000001E, 0x00000000, 0x00040047,
0x0000000D, 0x00000022, 0x00000001, 0x00040047,
0x0000000D, 0x00000021, 0x00000000, 0x00040047,
0x00000011, 0x0000001E, 0x00000000, 0x00040047,
0x00000015, 0x0000001E, 0x00000001, 0x00020013,
0x00000002, 0x00030021, 0x00000003, 0x00000002,
0x00030016, 0x00000006, 0x00000020, 0x00040017,
0x00000007, 0x00000006, 0x00000004, 0x00040020,
0x00000008, 0x00000003, 0x00000007, 0x0004003B,
0x00000008, 0x00000009, 0x00000003, 0x00090019,
0x0000000A, 0x00000006, 0x00000001, 0x00000000,
0x00000000, 0x00000000, 0x00000001, 0x00000000,
0x0003001B, 0x0000000B, 0x0000000A, 0x00040020,
0x0000000C, 0x00000000, 0x0000000B, 0x0004003B,
0x0000000C, 0x0000000D, 0x00000000, 0x00040017,
0x0000000F, 0x00000006, 0x00000002, 0x00040020,
0x00000010, 0x00000001, 0x0000000F, 0x0004003B,
0x00000010, 0x00000011, 0x00000001, 0x00040020,
0x00000014, 0x00000001, 0x00000007, 0x0004003B,
0x00000014, 0x00000015, 0x00000001, 0x00050036,
0x00000002, 0x00000004, 0x00000000, 0x00000003,
0x000200F8, 0x00000005, 0x0004003D, 0x0000000B,
0x0000000E, 0x0000000D, 0x0004003D, 0x0000000F,
0x00000012, 0x00000011, 0x00050057, 0x00000007,
0x00000013, 0x0000000E, 0x00000012, 0x0004003D,
0x00000007, 0x00000016, 0x00000015, 0x00050085,
0x00000007, 0x00000017, 0x00000013, 0x00000016,
0x0003003E, 0x00000009, 0x00000017, 0x000100FD,
0x00010038
} } }
};

#endif
