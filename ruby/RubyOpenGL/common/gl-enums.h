/* GL types - define if system GLheaders are not recent */

/* GL base */
#ifndef GL_VERSION_1_5
typedef ptrdiff_t GLintptr;
typedef ptrdiff_t GLsizeiptr;
#endif

#ifndef GL_VERSION_2_0
typedef char GLchar;
#endif

/* new GL types introduced by ARB extensions */
#ifndef GL_ARB_half_float_pixel
typedef unsigned short GLhalfARB;
#endif

#ifndef GL_ARB_shader_objects
typedef char GLcharARB;
typedef unsigned int GLhandleARB;
#endif

#ifndef GL_ARB_vertex_buffer_object
typedef ptrdiff_t GLintptrARB;
typedef ptrdiff_t GLsizeiptrARB;
#endif

/* new GL types introduced by other extensions */
#ifndef GL_NV_half_float
typedef unsigned short GLhalfNV;
#endif

/* List of GL enumerators */

/* The code below was genereated on Sun Feb 11 01:24:24 +0100 2007
   source: http://www.opengl.org/registry/api/enum.spec
           http://www.opengl.org/registry/api/enumext.spec
*/

#ifndef _GLENUMS_H_
#define _GLENUMS_H_ 

#ifndef GL_VERSION_1_1
#define GL_VERSION_1_1 1
#endif
#ifndef GL_VERSION_1_2
#define GL_VERSION_1_2 1
#endif
#ifndef GL_VERSION_1_3
#define GL_VERSION_1_3 1
#endif
#ifndef GL_VERSION_1_4
#define GL_VERSION_1_4 1
#endif
#ifndef GL_VERSION_1_5
#define GL_VERSION_1_5 1
#endif
#ifndef GL_ARB_imaging
#define GL_ARB_imaging 1
#endif
#ifndef GL_EXT_abgr
#define GL_EXT_abgr 1
#endif
#ifndef GL_EXT_blend_color
#define GL_EXT_blend_color 1
#endif
#ifndef GL_EXT_blend_logic_op
#define GL_EXT_blend_logic_op 1
#endif
#ifndef GL_EXT_blend_minmax
#define GL_EXT_blend_minmax 1
#endif
#ifndef GL_EXT_blend_subtract
#define GL_EXT_blend_subtract 1
#endif
#ifndef GL_EXT_cmyka
#define GL_EXT_cmyka 1
#endif
#ifndef GL_EXT_convolution
#define GL_EXT_convolution 1
#endif
#ifndef GL_EXT_copy_texture
#define GL_EXT_copy_texture 1
#endif
#ifndef GL_EXT_histogram
#define GL_EXT_histogram 1
#endif
#ifndef GL_EXT_packed_pixels
#define GL_EXT_packed_pixels 1
#endif
#ifndef GL_EXT_point_parameters
#define GL_EXT_point_parameters 1
#endif
#ifndef GL_EXT_polygon_offset
#define GL_EXT_polygon_offset 1
#endif
#ifndef GL_EXT_rescale_normal
#define GL_EXT_rescale_normal 1
#endif
#ifndef GL_EXT_shared_texture_palette
#define GL_EXT_shared_texture_palette 1
#endif
#ifndef GL_EXT_subtexture
#define GL_EXT_subtexture 1
#endif
#ifndef GL_EXT_texture
#define GL_EXT_texture 1
#endif
#ifndef GL_EXT_texture3D
#define GL_EXT_texture3D 1
#endif
#ifndef GL_EXT_texture_object
#define GL_EXT_texture_object 1
#endif
#ifndef GL_EXT_vertex_array
#define GL_EXT_vertex_array 1
#endif
#ifndef GL_SGIS_detail_texture
#define GL_SGIS_detail_texture 1
#endif
#ifndef GL_SGIS_fog_function
#define GL_SGIS_fog_function 1
#endif
#ifndef GL_SGIS_generate_mipmap
#define GL_SGIS_generate_mipmap 1
#endif
#ifndef GL_SGIS_multisample
#define GL_SGIS_multisample 1
#endif
#ifndef GL_SGIS_pixel_texture
#define GL_SGIS_pixel_texture 1
#endif
#ifndef GL_SGIS_point_line_texgen
#define GL_SGIS_point_line_texgen 1
#endif
#ifndef GL_SGIS_point_parameters
#define GL_SGIS_point_parameters 1
#endif
#ifndef GL_SGIS_sharpen_texture
#define GL_SGIS_sharpen_texture 1
#endif
#ifndef GL_SGIS_texture4D
#define GL_SGIS_texture4D 1
#endif
#ifndef GL_SGIS_texture_border_clamp
#define GL_SGIS_texture_border_clamp 1
#endif
#ifndef GL_SGIS_texture_edge_clamp
#define GL_SGIS_texture_edge_clamp 1
#endif
#ifndef GL_SGIS_texture_filter4
#define GL_SGIS_texture_filter4 1
#endif
#ifndef GL_SGIS_texture_lod
#define GL_SGIS_texture_lod 1
#endif
#ifndef GL_SGIS_texture_select
#define GL_SGIS_texture_select 1
#endif
#ifndef GL_SGIX_async
#define GL_SGIX_async 1
#endif
#ifndef GL_SGIX_async_histogram
#define GL_SGIX_async_histogram 1
#endif
#ifndef GL_SGIX_async_pixel
#define GL_SGIX_async_pixel 1
#endif
#ifndef GL_SGIX_blend_alpha_minmax
#define GL_SGIX_blend_alpha_minmax 1
#endif
#ifndef GL_SGIX_calligraphic_fragment
#define GL_SGIX_calligraphic_fragment 1
#endif
#ifndef GL_SGIX_clipmap
#define GL_SGIX_clipmap 1
#endif
#ifndef GL_SGIX_convolution_accuracy
#define GL_SGIX_convolution_accuracy 1
#endif
#ifndef GL_SGIX_depth_texture
#define GL_SGIX_depth_texture 1
#endif
#ifndef GL_SGIX_flush_raster
#define GL_SGIX_flush_raster 1
#endif
#ifndef GL_SGIX_fog_offset
#define GL_SGIX_fog_offset 1
#endif
#ifndef GL_SGIX_fragment_lighting
#define GL_SGIX_fragment_lighting 1
#endif
#ifndef GL_SGIX_framezoom
#define GL_SGIX_framezoom 1
#endif
#ifndef GL_SGIX_icc_texture
#define GL_SGIX_icc_texture 1
#endif
#ifndef GL_SGIX_impact_pixel_texture
#define GL_SGIX_impact_pixel_texture 1
#endif
#ifndef GL_SGIX_instruments
#define GL_SGIX_instruments 1
#endif
#ifndef GL_SGIX_interlace
#define GL_SGIX_interlace 1
#endif
#ifndef GL_SGIX_ir_instrument1
#define GL_SGIX_ir_instrument1 1
#endif
#ifndef GL_SGIX_list_priority
#define GL_SGIX_list_priority 1
#endif
#ifndef GL_SGIX_pixel_texture
#define GL_SGIX_pixel_texture 1
#endif
#ifndef GL_SGIX_pixel_tiles
#define GL_SGIX_pixel_tiles 1
#endif
#ifndef GL_SGIX_polynomial_ffd
#define GL_SGIX_polynomial_ffd 1
#endif
#ifndef GL_SGIX_reference_plane
#define GL_SGIX_reference_plane 1
#endif
#ifndef GL_SGIX_resample
#define GL_SGIX_resample 1
#endif
#ifndef GL_SGIX_scalebias_hint
#define GL_SGIX_scalebias_hint 1
#endif
#ifndef GL_SGIX_shadow
#define GL_SGIX_shadow 1
#endif
#ifndef GL_SGIX_shadow_ambient
#define GL_SGIX_shadow_ambient 1
#endif
#ifndef GL_SGIX_sprite
#define GL_SGIX_sprite 1
#endif
#ifndef GL_SGIX_subsample
#define GL_SGIX_subsample 1
#endif
#ifndef GL_SGIX_tag_sample_buffer
#define GL_SGIX_tag_sample_buffer 1
#endif
#ifndef GL_SGIX_texture_add_env
#define GL_SGIX_texture_add_env 1
#endif
#ifndef GL_SGIX_texture_coordinate_clamp
#define GL_SGIX_texture_coordinate_clamp 1
#endif
#ifndef GL_SGIX_texture_lod_bias
#define GL_SGIX_texture_lod_bias 1
#endif
#ifndef GL_SGIX_texture_multi_buffer
#define GL_SGIX_texture_multi_buffer 1
#endif
#ifndef GL_SGIX_texture_scale_bias
#define GL_SGIX_texture_scale_bias 1
#endif
#ifndef GL_SGIX_vertex_preclip
#define GL_SGIX_vertex_preclip 1
#endif
#ifndef GL_SGIX_ycrcb
#define GL_SGIX_ycrcb 1
#endif
#ifndef GL_SGI_color_matrix
#define GL_SGI_color_matrix 1
#endif
#ifndef GL_SGI_color_table
#define GL_SGI_color_table 1
#endif
#ifndef GL_SGI_texture_color_table
#define GL_SGI_texture_color_table 1
#endif
#ifndef GL_CURRENT_BIT
#define GL_CURRENT_BIT 0x00000001
#endif
#ifndef GL_POINT_BIT
#define GL_POINT_BIT 0x00000002
#endif
#ifndef GL_LINE_BIT
#define GL_LINE_BIT 0x00000004
#endif
#ifndef GL_POLYGON_BIT
#define GL_POLYGON_BIT 0x00000008
#endif
#ifndef GL_POLYGON_STIPPLE_BIT
#define GL_POLYGON_STIPPLE_BIT 0x00000010
#endif
#ifndef GL_PIXEL_MODE_BIT
#define GL_PIXEL_MODE_BIT 0x00000020
#endif
#ifndef GL_LIGHTING_BIT
#define GL_LIGHTING_BIT 0x00000040
#endif
#ifndef GL_FOG_BIT
#define GL_FOG_BIT 0x00000080
#endif
#ifndef GL_DEPTH_BUFFER_BIT
#define GL_DEPTH_BUFFER_BIT 0x00000100
#endif
#ifndef GL_ACCUM_BUFFER_BIT
#define GL_ACCUM_BUFFER_BIT 0x00000200
#endif
#ifndef GL_STENCIL_BUFFER_BIT
#define GL_STENCIL_BUFFER_BIT 0x00000400
#endif
#ifndef GL_VIEWPORT_BIT
#define GL_VIEWPORT_BIT 0x00000800
#endif
#ifndef GL_TRANSFORM_BIT
#define GL_TRANSFORM_BIT 0x00001000
#endif
#ifndef GL_ENABLE_BIT
#define GL_ENABLE_BIT 0x00002000
#endif
#ifndef GL_COLOR_BUFFER_BIT
#define GL_COLOR_BUFFER_BIT 0x00004000
#endif
#ifndef GL_HINT_BIT
#define GL_HINT_BIT 0x00008000
#endif
#ifndef GL_EVAL_BIT
#define GL_EVAL_BIT 0x00010000
#endif
#ifndef GL_LIST_BIT
#define GL_LIST_BIT 0x00020000
#endif
#ifndef GL_TEXTURE_BIT
#define GL_TEXTURE_BIT 0x00040000
#endif
#ifndef GL_SCISSOR_BIT
#define GL_SCISSOR_BIT 0x00080000
#endif
#ifndef GL_ALL_ATTRIB_BITS
#define GL_ALL_ATTRIB_BITS 0xFFFFFFFF
#endif
#ifndef GL_CLIENT_PIXEL_STORE_BIT
#define GL_CLIENT_PIXEL_STORE_BIT 0x00000001
#endif
#ifndef GL_CLIENT_VERTEX_ARRAY_BIT
#define GL_CLIENT_VERTEX_ARRAY_BIT 0x00000002
#endif
#ifndef GL_CLIENT_ALL_ATTRIB_BITS
#define GL_CLIENT_ALL_ATTRIB_BITS 0xFFFFFFFF
#endif
#ifndef GL_FALSE
#define GL_FALSE 0
#endif
#ifndef GL_TRUE
#define GL_TRUE 1
#endif
#ifndef GL_POINTS
#define GL_POINTS 0x0000
#endif
#ifndef GL_LINES
#define GL_LINES 0x0001
#endif
#ifndef GL_LINE_LOOP
#define GL_LINE_LOOP 0x0002
#endif
#ifndef GL_LINE_STRIP
#define GL_LINE_STRIP 0x0003
#endif
#ifndef GL_TRIANGLES
#define GL_TRIANGLES 0x0004
#endif
#ifndef GL_TRIANGLE_STRIP
#define GL_TRIANGLE_STRIP 0x0005
#endif
#ifndef GL_TRIANGLE_FAN
#define GL_TRIANGLE_FAN 0x0006
#endif
#ifndef GL_QUADS
#define GL_QUADS 0x0007
#endif
#ifndef GL_QUAD_STRIP
#define GL_QUAD_STRIP 0x0008
#endif
#ifndef GL_POLYGON
#define GL_POLYGON 0x0009
#endif
#ifndef GL_ACCUM
#define GL_ACCUM 0x0100
#endif
#ifndef GL_LOAD
#define GL_LOAD 0x0101
#endif
#ifndef GL_RETURN
#define GL_RETURN 0x0102
#endif
#ifndef GL_MULT
#define GL_MULT 0x0103
#endif
#ifndef GL_ADD
#define GL_ADD 0x0104
#endif
#ifndef GL_NEVER
#define GL_NEVER 0x0200
#endif
#ifndef GL_LESS
#define GL_LESS 0x0201
#endif
#ifndef GL_EQUAL
#define GL_EQUAL 0x0202
#endif
#ifndef GL_LEQUAL
#define GL_LEQUAL 0x0203
#endif
#ifndef GL_GREATER
#define GL_GREATER 0x0204
#endif
#ifndef GL_NOTEQUAL
#define GL_NOTEQUAL 0x0205
#endif
#ifndef GL_GEQUAL
#define GL_GEQUAL 0x0206
#endif
#ifndef GL_ALWAYS
#define GL_ALWAYS 0x0207
#endif
#ifndef GL_ZERO
#define GL_ZERO 0
#endif
#ifndef GL_ONE
#define GL_ONE 1
#endif
#ifndef GL_SRC_COLOR
#define GL_SRC_COLOR 0x0300
#endif
#ifndef GL_ONE_MINUS_SRC_COLOR
#define GL_ONE_MINUS_SRC_COLOR 0x0301
#endif
#ifndef GL_SRC_ALPHA
#define GL_SRC_ALPHA 0x0302
#endif
#ifndef GL_ONE_MINUS_SRC_ALPHA
#define GL_ONE_MINUS_SRC_ALPHA 0x0303
#endif
#ifndef GL_DST_ALPHA
#define GL_DST_ALPHA 0x0304
#endif
#ifndef GL_ONE_MINUS_DST_ALPHA
#define GL_ONE_MINUS_DST_ALPHA 0x0305
#endif
#ifndef GL_DST_COLOR
#define GL_DST_COLOR 0x0306
#endif
#ifndef GL_ONE_MINUS_DST_COLOR
#define GL_ONE_MINUS_DST_COLOR 0x0307
#endif
#ifndef GL_SRC_ALPHA_SATURATE
#define GL_SRC_ALPHA_SATURATE 0x0308
#endif
#ifndef GL_NONE
#define GL_NONE 0
#endif
#ifndef GL_FRONT_LEFT
#define GL_FRONT_LEFT 0x0400
#endif
#ifndef GL_FRONT_RIGHT
#define GL_FRONT_RIGHT 0x0401
#endif
#ifndef GL_BACK_LEFT
#define GL_BACK_LEFT 0x0402
#endif
#ifndef GL_BACK_RIGHT
#define GL_BACK_RIGHT 0x0403
#endif
#ifndef GL_FRONT
#define GL_FRONT 0x0404
#endif
#ifndef GL_BACK
#define GL_BACK 0x0405
#endif
#ifndef GL_LEFT
#define GL_LEFT 0x0406
#endif
#ifndef GL_RIGHT
#define GL_RIGHT 0x0407
#endif
#ifndef GL_FRONT_AND_BACK
#define GL_FRONT_AND_BACK 0x0408
#endif
#ifndef GL_AUX0
#define GL_AUX0 0x0409
#endif
#ifndef GL_AUX1
#define GL_AUX1 0x040A
#endif
#ifndef GL_AUX2
#define GL_AUX2 0x040B
#endif
#ifndef GL_AUX3
#define GL_AUX3 0x040C
#endif
#ifndef GL_NO_ERROR
#define GL_NO_ERROR 0
#endif
#ifndef GL_INVALID_ENUM
#define GL_INVALID_ENUM 0x0500
#endif
#ifndef GL_INVALID_VALUE
#define GL_INVALID_VALUE 0x0501
#endif
#ifndef GL_INVALID_OPERATION
#define GL_INVALID_OPERATION 0x0502
#endif
#ifndef GL_STACK_OVERFLOW
#define GL_STACK_OVERFLOW 0x0503
#endif
#ifndef GL_STACK_UNDERFLOW
#define GL_STACK_UNDERFLOW 0x0504
#endif
#ifndef GL_OUT_OF_MEMORY
#define GL_OUT_OF_MEMORY 0x0505
#endif
#ifndef GL_2D
#define GL_2D 0x0600
#endif
#ifndef GL_3D
#define GL_3D 0x0601
#endif
#ifndef GL_3D_COLOR
#define GL_3D_COLOR 0x0602
#endif
#ifndef GL_3D_COLOR_TEXTURE
#define GL_3D_COLOR_TEXTURE 0x0603
#endif
#ifndef GL_4D_COLOR_TEXTURE
#define GL_4D_COLOR_TEXTURE 0x0604
#endif
#ifndef GL_PASS_THROUGH_TOKEN
#define GL_PASS_THROUGH_TOKEN 0x0700
#endif
#ifndef GL_POINT_TOKEN
#define GL_POINT_TOKEN 0x0701
#endif
#ifndef GL_LINE_TOKEN
#define GL_LINE_TOKEN 0x0702
#endif
#ifndef GL_POLYGON_TOKEN
#define GL_POLYGON_TOKEN 0x0703
#endif
#ifndef GL_BITMAP_TOKEN
#define GL_BITMAP_TOKEN 0x0704
#endif
#ifndef GL_DRAW_PIXEL_TOKEN
#define GL_DRAW_PIXEL_TOKEN 0x0705
#endif
#ifndef GL_COPY_PIXEL_TOKEN
#define GL_COPY_PIXEL_TOKEN 0x0706
#endif
#ifndef GL_LINE_RESET_TOKEN
#define GL_LINE_RESET_TOKEN 0x0707
#endif
#ifndef GL_TEXTURE_DEFORMATION_BIT_SGIX
#define GL_TEXTURE_DEFORMATION_BIT_SGIX 0x00000001
#endif
#ifndef GL_GEOMETRY_DEFORMATION_BIT_SGIX
#define GL_GEOMETRY_DEFORMATION_BIT_SGIX 0x00000002
#endif
#ifndef GL_EXP
#define GL_EXP 0x0800
#endif
#ifndef GL_EXP2
#define GL_EXP2 0x0801
#endif
#ifndef GL_CW
#define GL_CW 0x0900
#endif
#ifndef GL_CCW
#define GL_CCW 0x0901
#endif
#ifndef GL_COEFF
#define GL_COEFF 0x0A00
#endif
#ifndef GL_ORDER
#define GL_ORDER 0x0A01
#endif
#ifndef GL_DOMAIN
#define GL_DOMAIN 0x0A02
#endif
#ifndef GL_PIXEL_MAP_I_TO_I
#define GL_PIXEL_MAP_I_TO_I 0x0C70
#endif
#ifndef GL_PIXEL_MAP_S_TO_S
#define GL_PIXEL_MAP_S_TO_S 0x0C71
#endif
#ifndef GL_PIXEL_MAP_I_TO_R
#define GL_PIXEL_MAP_I_TO_R 0x0C72
#endif
#ifndef GL_PIXEL_MAP_I_TO_G
#define GL_PIXEL_MAP_I_TO_G 0x0C73
#endif
#ifndef GL_PIXEL_MAP_I_TO_B
#define GL_PIXEL_MAP_I_TO_B 0x0C74
#endif
#ifndef GL_PIXEL_MAP_I_TO_A
#define GL_PIXEL_MAP_I_TO_A 0x0C75
#endif
#ifndef GL_PIXEL_MAP_R_TO_R
#define GL_PIXEL_MAP_R_TO_R 0x0C76
#endif
#ifndef GL_PIXEL_MAP_G_TO_G
#define GL_PIXEL_MAP_G_TO_G 0x0C77
#endif
#ifndef GL_PIXEL_MAP_B_TO_B
#define GL_PIXEL_MAP_B_TO_B 0x0C78
#endif
#ifndef GL_PIXEL_MAP_A_TO_A
#define GL_PIXEL_MAP_A_TO_A 0x0C79
#endif
#ifndef GL_VERTEX_ARRAY_POINTER
#define GL_VERTEX_ARRAY_POINTER 0x808E
#endif
#ifndef GL_NORMAL_ARRAY_POINTER
#define GL_NORMAL_ARRAY_POINTER 0x808F
#endif
#ifndef GL_COLOR_ARRAY_POINTER
#define GL_COLOR_ARRAY_POINTER 0x8090
#endif
#ifndef GL_INDEX_ARRAY_POINTER
#define GL_INDEX_ARRAY_POINTER 0x8091
#endif
#ifndef GL_TEXTURE_COORD_ARRAY_POINTER
#define GL_TEXTURE_COORD_ARRAY_POINTER 0x8092
#endif
#ifndef GL_EDGE_FLAG_ARRAY_POINTER
#define GL_EDGE_FLAG_ARRAY_POINTER 0x8093
#endif
#ifndef GL_FEEDBACK_BUFFER_POINTER
#define GL_FEEDBACK_BUFFER_POINTER 0x0DF0
#endif
#ifndef GL_SELECTION_BUFFER_POINTER
#define GL_SELECTION_BUFFER_POINTER 0x0DF3
#endif
#ifndef GL_CURRENT_COLOR
#define GL_CURRENT_COLOR 0x0B00
#endif
#ifndef GL_CURRENT_INDEX
#define GL_CURRENT_INDEX 0x0B01
#endif
#ifndef GL_CURRENT_NORMAL
#define GL_CURRENT_NORMAL 0x0B02
#endif
#ifndef GL_CURRENT_TEXTURE_COORDS
#define GL_CURRENT_TEXTURE_COORDS 0x0B03
#endif
#ifndef GL_CURRENT_RASTER_COLOR
#define GL_CURRENT_RASTER_COLOR 0x0B04
#endif
#ifndef GL_CURRENT_RASTER_INDEX
#define GL_CURRENT_RASTER_INDEX 0x0B05
#endif
#ifndef GL_CURRENT_RASTER_TEXTURE_COORDS
#define GL_CURRENT_RASTER_TEXTURE_COORDS 0x0B06
#endif
#ifndef GL_CURRENT_RASTER_POSITION
#define GL_CURRENT_RASTER_POSITION 0x0B07
#endif
#ifndef GL_CURRENT_RASTER_POSITION_VALID
#define GL_CURRENT_RASTER_POSITION_VALID 0x0B08
#endif
#ifndef GL_CURRENT_RASTER_DISTANCE
#define GL_CURRENT_RASTER_DISTANCE 0x0B09
#endif
#ifndef GL_POINT_SMOOTH
#define GL_POINT_SMOOTH 0x0B10
#endif
#ifndef GL_POINT_SIZE
#define GL_POINT_SIZE 0x0B11
#endif
#ifndef GL_POINT_SIZE_RANGE
#define GL_POINT_SIZE_RANGE 0x0B12
#endif
#ifndef GL_POINT_SIZE_GRANULARITY
#define GL_POINT_SIZE_GRANULARITY 0x0B13
#endif
#ifndef GL_LINE_SMOOTH
#define GL_LINE_SMOOTH 0x0B20
#endif
#ifndef GL_LINE_WIDTH
#define GL_LINE_WIDTH 0x0B21
#endif
#ifndef GL_LINE_WIDTH_RANGE
#define GL_LINE_WIDTH_RANGE 0x0B22
#endif
#ifndef GL_LINE_WIDTH_GRANULARITY
#define GL_LINE_WIDTH_GRANULARITY 0x0B23
#endif
#ifndef GL_LINE_STIPPLE
#define GL_LINE_STIPPLE 0x0B24
#endif
#ifndef GL_LINE_STIPPLE_PATTERN
#define GL_LINE_STIPPLE_PATTERN 0x0B25
#endif
#ifndef GL_LINE_STIPPLE_REPEAT
#define GL_LINE_STIPPLE_REPEAT 0x0B26
#endif
#ifndef GL_LIST_MODE
#define GL_LIST_MODE 0x0B30
#endif
#ifndef GL_MAX_LIST_NESTING
#define GL_MAX_LIST_NESTING 0x0B31
#endif
#ifndef GL_LIST_BASE
#define GL_LIST_BASE 0x0B32
#endif
#ifndef GL_LIST_INDEX
#define GL_LIST_INDEX 0x0B33
#endif
#ifndef GL_POLYGON_MODE
#define GL_POLYGON_MODE 0x0B40
#endif
#ifndef GL_POLYGON_SMOOTH
#define GL_POLYGON_SMOOTH 0x0B41
#endif
#ifndef GL_POLYGON_STIPPLE
#define GL_POLYGON_STIPPLE 0x0B42
#endif
#ifndef GL_EDGE_FLAG
#define GL_EDGE_FLAG 0x0B43
#endif
#ifndef GL_CULL_FACE
#define GL_CULL_FACE 0x0B44
#endif
#ifndef GL_CULL_FACE_MODE
#define GL_CULL_FACE_MODE 0x0B45
#endif
#ifndef GL_FRONT_FACE
#define GL_FRONT_FACE 0x0B46
#endif
#ifndef GL_LIGHTING
#define GL_LIGHTING 0x0B50
#endif
#ifndef GL_LIGHT_MODEL_LOCAL_VIEWER
#define GL_LIGHT_MODEL_LOCAL_VIEWER 0x0B51
#endif
#ifndef GL_LIGHT_MODEL_TWO_SIDE
#define GL_LIGHT_MODEL_TWO_SIDE 0x0B52
#endif
#ifndef GL_LIGHT_MODEL_AMBIENT
#define GL_LIGHT_MODEL_AMBIENT 0x0B53
#endif
#ifndef GL_SHADE_MODEL
#define GL_SHADE_MODEL 0x0B54
#endif
#ifndef GL_COLOR_MATERIAL_FACE
#define GL_COLOR_MATERIAL_FACE 0x0B55
#endif
#ifndef GL_COLOR_MATERIAL_PARAMETER
#define GL_COLOR_MATERIAL_PARAMETER 0x0B56
#endif
#ifndef GL_COLOR_MATERIAL
#define GL_COLOR_MATERIAL 0x0B57
#endif
#ifndef GL_FOG
#define GL_FOG 0x0B60
#endif
#ifndef GL_FOG_INDEX
#define GL_FOG_INDEX 0x0B61
#endif
#ifndef GL_FOG_DENSITY
#define GL_FOG_DENSITY 0x0B62
#endif
#ifndef GL_FOG_START
#define GL_FOG_START 0x0B63
#endif
#ifndef GL_FOG_END
#define GL_FOG_END 0x0B64
#endif
#ifndef GL_FOG_MODE
#define GL_FOG_MODE 0x0B65
#endif
#ifndef GL_FOG_COLOR
#define GL_FOG_COLOR 0x0B66
#endif
#ifndef GL_DEPTH_RANGE
#define GL_DEPTH_RANGE 0x0B70
#endif
#ifndef GL_DEPTH_TEST
#define GL_DEPTH_TEST 0x0B71
#endif
#ifndef GL_DEPTH_WRITEMASK
#define GL_DEPTH_WRITEMASK 0x0B72
#endif
#ifndef GL_DEPTH_CLEAR_VALUE
#define GL_DEPTH_CLEAR_VALUE 0x0B73
#endif
#ifndef GL_DEPTH_FUNC
#define GL_DEPTH_FUNC 0x0B74
#endif
#ifndef GL_ACCUM_CLEAR_VALUE
#define GL_ACCUM_CLEAR_VALUE 0x0B80
#endif
#ifndef GL_STENCIL_TEST
#define GL_STENCIL_TEST 0x0B90
#endif
#ifndef GL_STENCIL_CLEAR_VALUE
#define GL_STENCIL_CLEAR_VALUE 0x0B91
#endif
#ifndef GL_STENCIL_FUNC
#define GL_STENCIL_FUNC 0x0B92
#endif
#ifndef GL_STENCIL_VALUE_MASK
#define GL_STENCIL_VALUE_MASK 0x0B93
#endif
#ifndef GL_STENCIL_FAIL
#define GL_STENCIL_FAIL 0x0B94
#endif
#ifndef GL_STENCIL_PASS_DEPTH_FAIL
#define GL_STENCIL_PASS_DEPTH_FAIL 0x0B95
#endif
#ifndef GL_STENCIL_PASS_DEPTH_PASS
#define GL_STENCIL_PASS_DEPTH_PASS 0x0B96
#endif
#ifndef GL_STENCIL_REF
#define GL_STENCIL_REF 0x0B97
#endif
#ifndef GL_STENCIL_WRITEMASK
#define GL_STENCIL_WRITEMASK 0x0B98
#endif
#ifndef GL_MATRIX_MODE
#define GL_MATRIX_MODE 0x0BA0
#endif
#ifndef GL_NORMALIZE
#define GL_NORMALIZE 0x0BA1
#endif
#ifndef GL_VIEWPORT
#define GL_VIEWPORT 0x0BA2
#endif
#ifndef GL_MODELVIEW_STACK_DEPTH
#define GL_MODELVIEW_STACK_DEPTH 0x0BA3
#endif
#ifndef GL_PROJECTION_STACK_DEPTH
#define GL_PROJECTION_STACK_DEPTH 0x0BA4
#endif
#ifndef GL_TEXTURE_STACK_DEPTH
#define GL_TEXTURE_STACK_DEPTH 0x0BA5
#endif
#ifndef GL_MODELVIEW_MATRIX
#define GL_MODELVIEW_MATRIX 0x0BA6
#endif
#ifndef GL_PROJECTION_MATRIX
#define GL_PROJECTION_MATRIX 0x0BA7
#endif
#ifndef GL_TEXTURE_MATRIX
#define GL_TEXTURE_MATRIX 0x0BA8
#endif
#ifndef GL_ATTRIB_STACK_DEPTH
#define GL_ATTRIB_STACK_DEPTH 0x0BB0
#endif
#ifndef GL_CLIENT_ATTRIB_STACK_DEPTH
#define GL_CLIENT_ATTRIB_STACK_DEPTH 0x0BB1
#endif
#ifndef GL_ALPHA_TEST
#define GL_ALPHA_TEST 0x0BC0
#endif
#ifndef GL_ALPHA_TEST_FUNC
#define GL_ALPHA_TEST_FUNC 0x0BC1
#endif
#ifndef GL_ALPHA_TEST_REF
#define GL_ALPHA_TEST_REF 0x0BC2
#endif
#ifndef GL_DITHER
#define GL_DITHER 0x0BD0
#endif
#ifndef GL_BLEND_DST
#define GL_BLEND_DST 0x0BE0
#endif
#ifndef GL_BLEND_SRC
#define GL_BLEND_SRC 0x0BE1
#endif
#ifndef GL_BLEND
#define GL_BLEND 0x0BE2
#endif
#ifndef GL_LOGIC_OP_MODE
#define GL_LOGIC_OP_MODE 0x0BF0
#endif
#ifndef GL_INDEX_LOGIC_OP
#define GL_INDEX_LOGIC_OP 0x0BF1
#endif
#ifndef GL_LOGIC_OP
#define GL_LOGIC_OP 0x0BF1
#endif
#ifndef GL_COLOR_LOGIC_OP
#define GL_COLOR_LOGIC_OP 0x0BF2
#endif
#ifndef GL_AUX_BUFFERS
#define GL_AUX_BUFFERS 0x0C00
#endif
#ifndef GL_DRAW_BUFFER
#define GL_DRAW_BUFFER 0x0C01
#endif
#ifndef GL_READ_BUFFER
#define GL_READ_BUFFER 0x0C02
#endif
#ifndef GL_SCISSOR_BOX
#define GL_SCISSOR_BOX 0x0C10
#endif
#ifndef GL_SCISSOR_TEST
#define GL_SCISSOR_TEST 0x0C11
#endif
#ifndef GL_INDEX_CLEAR_VALUE
#define GL_INDEX_CLEAR_VALUE 0x0C20
#endif
#ifndef GL_INDEX_WRITEMASK
#define GL_INDEX_WRITEMASK 0x0C21
#endif
#ifndef GL_COLOR_CLEAR_VALUE
#define GL_COLOR_CLEAR_VALUE 0x0C22
#endif
#ifndef GL_COLOR_WRITEMASK
#define GL_COLOR_WRITEMASK 0x0C23
#endif
#ifndef GL_INDEX_MODE
#define GL_INDEX_MODE 0x0C30
#endif
#ifndef GL_RGBA_MODE
#define GL_RGBA_MODE 0x0C31
#endif
#ifndef GL_DOUBLEBUFFER
#define GL_DOUBLEBUFFER 0x0C32
#endif
#ifndef GL_STEREO
#define GL_STEREO 0x0C33
#endif
#ifndef GL_RENDER_MODE
#define GL_RENDER_MODE 0x0C40
#endif
#ifndef GL_PERSPECTIVE_CORRECTION_HINT
#define GL_PERSPECTIVE_CORRECTION_HINT 0x0C50
#endif
#ifndef GL_POINT_SMOOTH_HINT
#define GL_POINT_SMOOTH_HINT 0x0C51
#endif
#ifndef GL_LINE_SMOOTH_HINT
#define GL_LINE_SMOOTH_HINT 0x0C52
#endif
#ifndef GL_POLYGON_SMOOTH_HINT
#define GL_POLYGON_SMOOTH_HINT 0x0C53
#endif
#ifndef GL_FOG_HINT
#define GL_FOG_HINT 0x0C54
#endif
#ifndef GL_TEXTURE_GEN_S
#define GL_TEXTURE_GEN_S 0x0C60
#endif
#ifndef GL_TEXTURE_GEN_T
#define GL_TEXTURE_GEN_T 0x0C61
#endif
#ifndef GL_TEXTURE_GEN_R
#define GL_TEXTURE_GEN_R 0x0C62
#endif
#ifndef GL_TEXTURE_GEN_Q
#define GL_TEXTURE_GEN_Q 0x0C63
#endif
#ifndef GL_PIXEL_MAP_I_TO_I_SIZE
#define GL_PIXEL_MAP_I_TO_I_SIZE 0x0CB0
#endif
#ifndef GL_PIXEL_MAP_S_TO_S_SIZE
#define GL_PIXEL_MAP_S_TO_S_SIZE 0x0CB1
#endif
#ifndef GL_PIXEL_MAP_I_TO_R_SIZE
#define GL_PIXEL_MAP_I_TO_R_SIZE 0x0CB2
#endif
#ifndef GL_PIXEL_MAP_I_TO_G_SIZE
#define GL_PIXEL_MAP_I_TO_G_SIZE 0x0CB3
#endif
#ifndef GL_PIXEL_MAP_I_TO_B_SIZE
#define GL_PIXEL_MAP_I_TO_B_SIZE 0x0CB4
#endif
#ifndef GL_PIXEL_MAP_I_TO_A_SIZE
#define GL_PIXEL_MAP_I_TO_A_SIZE 0x0CB5
#endif
#ifndef GL_PIXEL_MAP_R_TO_R_SIZE
#define GL_PIXEL_MAP_R_TO_R_SIZE 0x0CB6
#endif
#ifndef GL_PIXEL_MAP_G_TO_G_SIZE
#define GL_PIXEL_MAP_G_TO_G_SIZE 0x0CB7
#endif
#ifndef GL_PIXEL_MAP_B_TO_B_SIZE
#define GL_PIXEL_MAP_B_TO_B_SIZE 0x0CB8
#endif
#ifndef GL_PIXEL_MAP_A_TO_A_SIZE
#define GL_PIXEL_MAP_A_TO_A_SIZE 0x0CB9
#endif
#ifndef GL_UNPACK_SWAP_BYTES
#define GL_UNPACK_SWAP_BYTES 0x0CF0
#endif
#ifndef GL_UNPACK_LSB_FIRST
#define GL_UNPACK_LSB_FIRST 0x0CF1
#endif
#ifndef GL_UNPACK_ROW_LENGTH
#define GL_UNPACK_ROW_LENGTH 0x0CF2
#endif
#ifndef GL_UNPACK_SKIP_ROWS
#define GL_UNPACK_SKIP_ROWS 0x0CF3
#endif
#ifndef GL_UNPACK_SKIP_PIXELS
#define GL_UNPACK_SKIP_PIXELS 0x0CF4
#endif
#ifndef GL_UNPACK_ALIGNMENT
#define GL_UNPACK_ALIGNMENT 0x0CF5
#endif
#ifndef GL_PACK_SWAP_BYTES
#define GL_PACK_SWAP_BYTES 0x0D00
#endif
#ifndef GL_PACK_LSB_FIRST
#define GL_PACK_LSB_FIRST 0x0D01
#endif
#ifndef GL_PACK_ROW_LENGTH
#define GL_PACK_ROW_LENGTH 0x0D02
#endif
#ifndef GL_PACK_SKIP_ROWS
#define GL_PACK_SKIP_ROWS 0x0D03
#endif
#ifndef GL_PACK_SKIP_PIXELS
#define GL_PACK_SKIP_PIXELS 0x0D04
#endif
#ifndef GL_PACK_ALIGNMENT
#define GL_PACK_ALIGNMENT 0x0D05
#endif
#ifndef GL_MAP_COLOR
#define GL_MAP_COLOR 0x0D10
#endif
#ifndef GL_MAP_STENCIL
#define GL_MAP_STENCIL 0x0D11
#endif
#ifndef GL_INDEX_SHIFT
#define GL_INDEX_SHIFT 0x0D12
#endif
#ifndef GL_INDEX_OFFSET
#define GL_INDEX_OFFSET 0x0D13
#endif
#ifndef GL_RED_SCALE
#define GL_RED_SCALE 0x0D14
#endif
#ifndef GL_RED_BIAS
#define GL_RED_BIAS 0x0D15
#endif
#ifndef GL_ZOOM_X
#define GL_ZOOM_X 0x0D16
#endif
#ifndef GL_ZOOM_Y
#define GL_ZOOM_Y 0x0D17
#endif
#ifndef GL_GREEN_SCALE
#define GL_GREEN_SCALE 0x0D18
#endif
#ifndef GL_GREEN_BIAS
#define GL_GREEN_BIAS 0x0D19
#endif
#ifndef GL_BLUE_SCALE
#define GL_BLUE_SCALE 0x0D1A
#endif
#ifndef GL_BLUE_BIAS
#define GL_BLUE_BIAS 0x0D1B
#endif
#ifndef GL_ALPHA_SCALE
#define GL_ALPHA_SCALE 0x0D1C
#endif
#ifndef GL_ALPHA_BIAS
#define GL_ALPHA_BIAS 0x0D1D
#endif
#ifndef GL_DEPTH_SCALE
#define GL_DEPTH_SCALE 0x0D1E
#endif
#ifndef GL_DEPTH_BIAS
#define GL_DEPTH_BIAS 0x0D1F
#endif
#ifndef GL_MAX_EVAL_ORDER
#define GL_MAX_EVAL_ORDER 0x0D30
#endif
#ifndef GL_MAX_LIGHTS
#define GL_MAX_LIGHTS 0x0D31
#endif
#ifndef GL_MAX_CLIP_PLANES
#define GL_MAX_CLIP_PLANES 0x0D32
#endif
#ifndef GL_MAX_TEXTURE_SIZE
#define GL_MAX_TEXTURE_SIZE 0x0D33
#endif
#ifndef GL_MAX_PIXEL_MAP_TABLE
#define GL_MAX_PIXEL_MAP_TABLE 0x0D34
#endif
#ifndef GL_MAX_ATTRIB_STACK_DEPTH
#define GL_MAX_ATTRIB_STACK_DEPTH 0x0D35
#endif
#ifndef GL_MAX_MODELVIEW_STACK_DEPTH
#define GL_MAX_MODELVIEW_STACK_DEPTH 0x0D36
#endif
#ifndef GL_MAX_NAME_STACK_DEPTH
#define GL_MAX_NAME_STACK_DEPTH 0x0D37
#endif
#ifndef GL_MAX_PROJECTION_STACK_DEPTH
#define GL_MAX_PROJECTION_STACK_DEPTH 0x0D38
#endif
#ifndef GL_MAX_TEXTURE_STACK_DEPTH
#define GL_MAX_TEXTURE_STACK_DEPTH 0x0D39
#endif
#ifndef GL_MAX_VIEWPORT_DIMS
#define GL_MAX_VIEWPORT_DIMS 0x0D3A
#endif
#ifndef GL_MAX_CLIENT_ATTRIB_STACK_DEPTH
#define GL_MAX_CLIENT_ATTRIB_STACK_DEPTH 0x0D3B
#endif
#ifndef GL_SUBPIXEL_BITS
#define GL_SUBPIXEL_BITS 0x0D50
#endif
#ifndef GL_INDEX_BITS
#define GL_INDEX_BITS 0x0D51
#endif
#ifndef GL_RED_BITS
#define GL_RED_BITS 0x0D52
#endif
#ifndef GL_GREEN_BITS
#define GL_GREEN_BITS 0x0D53
#endif
#ifndef GL_BLUE_BITS
#define GL_BLUE_BITS 0x0D54
#endif
#ifndef GL_ALPHA_BITS
#define GL_ALPHA_BITS 0x0D55
#endif
#ifndef GL_DEPTH_BITS
#define GL_DEPTH_BITS 0x0D56
#endif
#ifndef GL_STENCIL_BITS
#define GL_STENCIL_BITS 0x0D57
#endif
#ifndef GL_ACCUM_RED_BITS
#define GL_ACCUM_RED_BITS 0x0D58
#endif
#ifndef GL_ACCUM_GREEN_BITS
#define GL_ACCUM_GREEN_BITS 0x0D59
#endif
#ifndef GL_ACCUM_BLUE_BITS
#define GL_ACCUM_BLUE_BITS 0x0D5A
#endif
#ifndef GL_ACCUM_ALPHA_BITS
#define GL_ACCUM_ALPHA_BITS 0x0D5B
#endif
#ifndef GL_NAME_STACK_DEPTH
#define GL_NAME_STACK_DEPTH 0x0D70
#endif
#ifndef GL_AUTO_NORMAL
#define GL_AUTO_NORMAL 0x0D80
#endif
#ifndef GL_MAP1_COLOR_4
#define GL_MAP1_COLOR_4 0x0D90
#endif
#ifndef GL_MAP1_INDEX
#define GL_MAP1_INDEX 0x0D91
#endif
#ifndef GL_MAP1_NORMAL
#define GL_MAP1_NORMAL 0x0D92
#endif
#ifndef GL_MAP1_TEXTURE_COORD_1
#define GL_MAP1_TEXTURE_COORD_1 0x0D93
#endif
#ifndef GL_MAP1_TEXTURE_COORD_2
#define GL_MAP1_TEXTURE_COORD_2 0x0D94
#endif
#ifndef GL_MAP1_TEXTURE_COORD_3
#define GL_MAP1_TEXTURE_COORD_3 0x0D95
#endif
#ifndef GL_MAP1_TEXTURE_COORD_4
#define GL_MAP1_TEXTURE_COORD_4 0x0D96
#endif
#ifndef GL_MAP1_VERTEX_3
#define GL_MAP1_VERTEX_3 0x0D97
#endif
#ifndef GL_MAP1_VERTEX_4
#define GL_MAP1_VERTEX_4 0x0D98
#endif
#ifndef GL_MAP2_COLOR_4
#define GL_MAP2_COLOR_4 0x0DB0
#endif
#ifndef GL_MAP2_INDEX
#define GL_MAP2_INDEX 0x0DB1
#endif
#ifndef GL_MAP2_NORMAL
#define GL_MAP2_NORMAL 0x0DB2
#endif
#ifndef GL_MAP2_TEXTURE_COORD_1
#define GL_MAP2_TEXTURE_COORD_1 0x0DB3
#endif
#ifndef GL_MAP2_TEXTURE_COORD_2
#define GL_MAP2_TEXTURE_COORD_2 0x0DB4
#endif
#ifndef GL_MAP2_TEXTURE_COORD_3
#define GL_MAP2_TEXTURE_COORD_3 0x0DB5
#endif
#ifndef GL_MAP2_TEXTURE_COORD_4
#define GL_MAP2_TEXTURE_COORD_4 0x0DB6
#endif
#ifndef GL_MAP2_VERTEX_3
#define GL_MAP2_VERTEX_3 0x0DB7
#endif
#ifndef GL_MAP2_VERTEX_4
#define GL_MAP2_VERTEX_4 0x0DB8
#endif
#ifndef GL_MAP1_GRID_DOMAIN
#define GL_MAP1_GRID_DOMAIN 0x0DD0
#endif
#ifndef GL_MAP1_GRID_SEGMENTS
#define GL_MAP1_GRID_SEGMENTS 0x0DD1
#endif
#ifndef GL_MAP2_GRID_DOMAIN
#define GL_MAP2_GRID_DOMAIN 0x0DD2
#endif
#ifndef GL_MAP2_GRID_SEGMENTS
#define GL_MAP2_GRID_SEGMENTS 0x0DD3
#endif
#ifndef GL_TEXTURE_1D
#define GL_TEXTURE_1D 0x0DE0
#endif
#ifndef GL_TEXTURE_2D
#define GL_TEXTURE_2D 0x0DE1
#endif
#ifndef GL_FEEDBACK_BUFFER_SIZE
#define GL_FEEDBACK_BUFFER_SIZE 0x0DF1
#endif
#ifndef GL_FEEDBACK_BUFFER_TYPE
#define GL_FEEDBACK_BUFFER_TYPE 0x0DF2
#endif
#ifndef GL_SELECTION_BUFFER_SIZE
#define GL_SELECTION_BUFFER_SIZE 0x0DF4
#endif
#ifndef GL_POLYGON_OFFSET_UNITS
#define GL_POLYGON_OFFSET_UNITS 0x2A00
#endif
#ifndef GL_POLYGON_OFFSET_POINT
#define GL_POLYGON_OFFSET_POINT 0x2A01
#endif
#ifndef GL_POLYGON_OFFSET_LINE
#define GL_POLYGON_OFFSET_LINE 0x2A02
#endif
#ifndef GL_POLYGON_OFFSET_FILL
#define GL_POLYGON_OFFSET_FILL 0x8037
#endif
#ifndef GL_POLYGON_OFFSET_FACTOR
#define GL_POLYGON_OFFSET_FACTOR 0x8038
#endif
#ifndef GL_TEXTURE_BINDING_1D
#define GL_TEXTURE_BINDING_1D 0x8068
#endif
#ifndef GL_TEXTURE_BINDING_2D
#define GL_TEXTURE_BINDING_2D 0x8069
#endif
#ifndef GL_TEXTURE_BINDING_3D
#define GL_TEXTURE_BINDING_3D 0x806A
#endif
#ifndef GL_VERTEX_ARRAY
#define GL_VERTEX_ARRAY 0x8074
#endif
#ifndef GL_NORMAL_ARRAY
#define GL_NORMAL_ARRAY 0x8075
#endif
#ifndef GL_COLOR_ARRAY
#define GL_COLOR_ARRAY 0x8076
#endif
#ifndef GL_INDEX_ARRAY
#define GL_INDEX_ARRAY 0x8077
#endif
#ifndef GL_TEXTURE_COORD_ARRAY
#define GL_TEXTURE_COORD_ARRAY 0x8078
#endif
#ifndef GL_EDGE_FLAG_ARRAY
#define GL_EDGE_FLAG_ARRAY 0x8079
#endif
#ifndef GL_VERTEX_ARRAY_SIZE
#define GL_VERTEX_ARRAY_SIZE 0x807A
#endif
#ifndef GL_VERTEX_ARRAY_TYPE
#define GL_VERTEX_ARRAY_TYPE 0x807B
#endif
#ifndef GL_VERTEX_ARRAY_STRIDE
#define GL_VERTEX_ARRAY_STRIDE 0x807C
#endif
#ifndef GL_NORMAL_ARRAY_TYPE
#define GL_NORMAL_ARRAY_TYPE 0x807E
#endif
#ifndef GL_NORMAL_ARRAY_STRIDE
#define GL_NORMAL_ARRAY_STRIDE 0x807F
#endif
#ifndef GL_COLOR_ARRAY_SIZE
#define GL_COLOR_ARRAY_SIZE 0x8081
#endif
#ifndef GL_COLOR_ARRAY_TYPE
#define GL_COLOR_ARRAY_TYPE 0x8082
#endif
#ifndef GL_COLOR_ARRAY_STRIDE
#define GL_COLOR_ARRAY_STRIDE 0x8083
#endif
#ifndef GL_INDEX_ARRAY_TYPE
#define GL_INDEX_ARRAY_TYPE 0x8085
#endif
#ifndef GL_INDEX_ARRAY_STRIDE
#define GL_INDEX_ARRAY_STRIDE 0x8086
#endif
#ifndef GL_TEXTURE_COORD_ARRAY_SIZE
#define GL_TEXTURE_COORD_ARRAY_SIZE 0x8088
#endif
#ifndef GL_TEXTURE_COORD_ARRAY_TYPE
#define GL_TEXTURE_COORD_ARRAY_TYPE 0x8089
#endif
#ifndef GL_TEXTURE_COORD_ARRAY_STRIDE
#define GL_TEXTURE_COORD_ARRAY_STRIDE 0x808A
#endif
#ifndef GL_EDGE_FLAG_ARRAY_STRIDE
#define GL_EDGE_FLAG_ARRAY_STRIDE 0x808C
#endif
#ifndef GL_TEXTURE_WIDTH
#define GL_TEXTURE_WIDTH 0x1000
#endif
#ifndef GL_TEXTURE_HEIGHT
#define GL_TEXTURE_HEIGHT 0x1001
#endif
#ifndef GL_TEXTURE_INTERNAL_FORMAT
#define GL_TEXTURE_INTERNAL_FORMAT 0x1003
#endif
#ifndef GL_TEXTURE_COMPONENTS
#define GL_TEXTURE_COMPONENTS 0x1003
#endif
#ifndef GL_TEXTURE_BORDER_COLOR
#define GL_TEXTURE_BORDER_COLOR 0x1004
#endif
#ifndef GL_TEXTURE_BORDER
#define GL_TEXTURE_BORDER 0x1005
#endif
#ifndef GL_TEXTURE_RED_SIZE
#define GL_TEXTURE_RED_SIZE 0x805C
#endif
#ifndef GL_TEXTURE_GREEN_SIZE
#define GL_TEXTURE_GREEN_SIZE 0x805D
#endif
#ifndef GL_TEXTURE_BLUE_SIZE
#define GL_TEXTURE_BLUE_SIZE 0x805E
#endif
#ifndef GL_TEXTURE_ALPHA_SIZE
#define GL_TEXTURE_ALPHA_SIZE 0x805F
#endif
#ifndef GL_TEXTURE_LUMINANCE_SIZE
#define GL_TEXTURE_LUMINANCE_SIZE 0x8060
#endif
#ifndef GL_TEXTURE_INTENSITY_SIZE
#define GL_TEXTURE_INTENSITY_SIZE 0x8061
#endif
#ifndef GL_TEXTURE_PRIORITY
#define GL_TEXTURE_PRIORITY 0x8066
#endif
#ifndef GL_TEXTURE_RESIDENT
#define GL_TEXTURE_RESIDENT 0x8067
#endif
#ifndef GL_DONT_CARE
#define GL_DONT_CARE 0x1100
#endif
#ifndef GL_FASTEST
#define GL_FASTEST 0x1101
#endif
#ifndef GL_NICEST
#define GL_NICEST 0x1102
#endif
#ifndef GL_AMBIENT
#define GL_AMBIENT 0x1200
#endif
#ifndef GL_DIFFUSE
#define GL_DIFFUSE 0x1201
#endif
#ifndef GL_SPECULAR
#define GL_SPECULAR 0x1202
#endif
#ifndef GL_POSITION
#define GL_POSITION 0x1203
#endif
#ifndef GL_SPOT_DIRECTION
#define GL_SPOT_DIRECTION 0x1204
#endif
#ifndef GL_SPOT_EXPONENT
#define GL_SPOT_EXPONENT 0x1205
#endif
#ifndef GL_SPOT_CUTOFF
#define GL_SPOT_CUTOFF 0x1206
#endif
#ifndef GL_CONSTANT_ATTENUATION
#define GL_CONSTANT_ATTENUATION 0x1207
#endif
#ifndef GL_LINEAR_ATTENUATION
#define GL_LINEAR_ATTENUATION 0x1208
#endif
#ifndef GL_QUADRATIC_ATTENUATION
#define GL_QUADRATIC_ATTENUATION 0x1209
#endif
#ifndef GL_COMPILE
#define GL_COMPILE 0x1300
#endif
#ifndef GL_COMPILE_AND_EXECUTE
#define GL_COMPILE_AND_EXECUTE 0x1301
#endif
#ifndef GL_BYTE
#define GL_BYTE 0x1400
#endif
#ifndef GL_UNSIGNED_BYTE
#define GL_UNSIGNED_BYTE 0x1401
#endif
#ifndef GL_SHORT
#define GL_SHORT 0x1402
#endif
#ifndef GL_UNSIGNED_SHORT
#define GL_UNSIGNED_SHORT 0x1403
#endif
#ifndef GL_INT
#define GL_INT 0x1404
#endif
#ifndef GL_UNSIGNED_INT
#define GL_UNSIGNED_INT 0x1405
#endif
#ifndef GL_FLOAT
#define GL_FLOAT 0x1406
#endif
#ifndef GL_2_BYTES
#define GL_2_BYTES 0x1407
#endif
#ifndef GL_3_BYTES
#define GL_3_BYTES 0x1408
#endif
#ifndef GL_4_BYTES
#define GL_4_BYTES 0x1409
#endif
#ifndef GL_DOUBLE
#define GL_DOUBLE 0x140A
#endif
#ifndef GL_DOUBLE_EXT
#define GL_DOUBLE_EXT 0x140A
#endif
#ifndef GL_CLEAR
#define GL_CLEAR 0x1500
#endif
#ifndef GL_AND
#define GL_AND 0x1501
#endif
#ifndef GL_AND_REVERSE
#define GL_AND_REVERSE 0x1502
#endif
#ifndef GL_COPY
#define GL_COPY 0x1503
#endif
#ifndef GL_AND_INVERTED
#define GL_AND_INVERTED 0x1504
#endif
#ifndef GL_NOOP
#define GL_NOOP 0x1505
#endif
#ifndef GL_XOR
#define GL_XOR 0x1506
#endif
#ifndef GL_OR
#define GL_OR 0x1507
#endif
#ifndef GL_NOR
#define GL_NOR 0x1508
#endif
#ifndef GL_EQUIV
#define GL_EQUIV 0x1509
#endif
#ifndef GL_INVERT
#define GL_INVERT 0x150A
#endif
#ifndef GL_OR_REVERSE
#define GL_OR_REVERSE 0x150B
#endif
#ifndef GL_COPY_INVERTED
#define GL_COPY_INVERTED 0x150C
#endif
#ifndef GL_OR_INVERTED
#define GL_OR_INVERTED 0x150D
#endif
#ifndef GL_NAND
#define GL_NAND 0x150E
#endif
#ifndef GL_SET
#define GL_SET 0x150F
#endif
#ifndef GL_EMISSION
#define GL_EMISSION 0x1600
#endif
#ifndef GL_SHININESS
#define GL_SHININESS 0x1601
#endif
#ifndef GL_AMBIENT_AND_DIFFUSE
#define GL_AMBIENT_AND_DIFFUSE 0x1602
#endif
#ifndef GL_COLOR_INDEXES
#define GL_COLOR_INDEXES 0x1603
#endif
#ifndef GL_MODELVIEW
#define GL_MODELVIEW 0x1700
#endif
#ifndef GL_PROJECTION
#define GL_PROJECTION 0x1701
#endif
#ifndef GL_TEXTURE
#define GL_TEXTURE 0x1702
#endif
#ifndef GL_COLOR
#define GL_COLOR 0x1800
#endif
#ifndef GL_DEPTH
#define GL_DEPTH 0x1801
#endif
#ifndef GL_STENCIL
#define GL_STENCIL 0x1802
#endif
#ifndef GL_COLOR_INDEX
#define GL_COLOR_INDEX 0x1900
#endif
#ifndef GL_STENCIL_INDEX
#define GL_STENCIL_INDEX 0x1901
#endif
#ifndef GL_DEPTH_COMPONENT
#define GL_DEPTH_COMPONENT 0x1902
#endif
#ifndef GL_RED
#define GL_RED 0x1903
#endif
#ifndef GL_GREEN
#define GL_GREEN 0x1904
#endif
#ifndef GL_BLUE
#define GL_BLUE 0x1905
#endif
#ifndef GL_ALPHA
#define GL_ALPHA 0x1906
#endif
#ifndef GL_RGB
#define GL_RGB 0x1907
#endif
#ifndef GL_RGBA
#define GL_RGBA 0x1908
#endif
#ifndef GL_LUMINANCE
#define GL_LUMINANCE 0x1909
#endif
#ifndef GL_LUMINANCE_ALPHA
#define GL_LUMINANCE_ALPHA 0x190A
#endif
#ifndef GL_BITMAP
#define GL_BITMAP 0x1A00
#endif
#ifndef GL_POINT
#define GL_POINT 0x1B00
#endif
#ifndef GL_LINE
#define GL_LINE 0x1B01
#endif
#ifndef GL_FILL
#define GL_FILL 0x1B02
#endif
#ifndef GL_RENDER
#define GL_RENDER 0x1C00
#endif
#ifndef GL_FEEDBACK
#define GL_FEEDBACK 0x1C01
#endif
#ifndef GL_SELECT
#define GL_SELECT 0x1C02
#endif
#ifndef GL_FLAT
#define GL_FLAT 0x1D00
#endif
#ifndef GL_SMOOTH
#define GL_SMOOTH 0x1D01
#endif
#ifndef GL_KEEP
#define GL_KEEP 0x1E00
#endif
#ifndef GL_REPLACE
#define GL_REPLACE 0x1E01
#endif
#ifndef GL_INCR
#define GL_INCR 0x1E02
#endif
#ifndef GL_DECR
#define GL_DECR 0x1E03
#endif
#ifndef GL_VENDOR
#define GL_VENDOR 0x1F00
#endif
#ifndef GL_RENDERER
#define GL_RENDERER 0x1F01
#endif
#ifndef GL_VERSION
#define GL_VERSION 0x1F02
#endif
#ifndef GL_EXTENSIONS
#define GL_EXTENSIONS 0x1F03
#endif
#ifndef GL_S
#define GL_S 0x2000
#endif
#ifndef GL_T
#define GL_T 0x2001
#endif
#ifndef GL_R
#define GL_R 0x2002
#endif
#ifndef GL_Q
#define GL_Q 0x2003
#endif
#ifndef GL_MODULATE
#define GL_MODULATE 0x2100
#endif
#ifndef GL_DECAL
#define GL_DECAL 0x2101
#endif
#ifndef GL_TEXTURE_ENV_MODE
#define GL_TEXTURE_ENV_MODE 0x2200
#endif
#ifndef GL_TEXTURE_ENV_COLOR
#define GL_TEXTURE_ENV_COLOR 0x2201
#endif
#ifndef GL_TEXTURE_ENV
#define GL_TEXTURE_ENV 0x2300
#endif
#ifndef GL_EYE_LINEAR
#define GL_EYE_LINEAR 0x2400
#endif
#ifndef GL_OBJECT_LINEAR
#define GL_OBJECT_LINEAR 0x2401
#endif
#ifndef GL_SPHERE_MAP
#define GL_SPHERE_MAP 0x2402
#endif
#ifndef GL_TEXTURE_GEN_MODE
#define GL_TEXTURE_GEN_MODE 0x2500
#endif
#ifndef GL_OBJECT_PLANE
#define GL_OBJECT_PLANE 0x2501
#endif
#ifndef GL_EYE_PLANE
#define GL_EYE_PLANE 0x2502
#endif
#ifndef GL_NEAREST
#define GL_NEAREST 0x2600
#endif
#ifndef GL_LINEAR
#define GL_LINEAR 0x2601
#endif
#ifndef GL_NEAREST_MIPMAP_NEAREST
#define GL_NEAREST_MIPMAP_NEAREST 0x2700
#endif
#ifndef GL_LINEAR_MIPMAP_NEAREST
#define GL_LINEAR_MIPMAP_NEAREST 0x2701
#endif
#ifndef GL_NEAREST_MIPMAP_LINEAR
#define GL_NEAREST_MIPMAP_LINEAR 0x2702
#endif
#ifndef GL_LINEAR_MIPMAP_LINEAR
#define GL_LINEAR_MIPMAP_LINEAR 0x2703
#endif
#ifndef GL_TEXTURE_MAG_FILTER
#define GL_TEXTURE_MAG_FILTER 0x2800
#endif
#ifndef GL_TEXTURE_MIN_FILTER
#define GL_TEXTURE_MIN_FILTER 0x2801
#endif
#ifndef GL_TEXTURE_WRAP_S
#define GL_TEXTURE_WRAP_S 0x2802
#endif
#ifndef GL_TEXTURE_WRAP_T
#define GL_TEXTURE_WRAP_T 0x2803
#endif
#ifndef GL_PROXY_TEXTURE_1D
#define GL_PROXY_TEXTURE_1D 0x8063
#endif
#ifndef GL_PROXY_TEXTURE_2D
#define GL_PROXY_TEXTURE_2D 0x8064
#endif
#ifndef GL_CLAMP
#define GL_CLAMP 0x2900
#endif
#ifndef GL_REPEAT
#define GL_REPEAT 0x2901
#endif
#ifndef GL_R3_G3_B2
#define GL_R3_G3_B2 0x2A10
#endif
#ifndef GL_ALPHA4
#define GL_ALPHA4 0x803B
#endif
#ifndef GL_ALPHA8
#define GL_ALPHA8 0x803C
#endif
#ifndef GL_ALPHA12
#define GL_ALPHA12 0x803D
#endif
#ifndef GL_ALPHA16
#define GL_ALPHA16 0x803E
#endif
#ifndef GL_LUMINANCE4
#define GL_LUMINANCE4 0x803F
#endif
#ifndef GL_LUMINANCE8
#define GL_LUMINANCE8 0x8040
#endif
#ifndef GL_LUMINANCE12
#define GL_LUMINANCE12 0x8041
#endif
#ifndef GL_LUMINANCE16
#define GL_LUMINANCE16 0x8042
#endif
#ifndef GL_LUMINANCE4_ALPHA4
#define GL_LUMINANCE4_ALPHA4 0x8043
#endif
#ifndef GL_LUMINANCE6_ALPHA2
#define GL_LUMINANCE6_ALPHA2 0x8044
#endif
#ifndef GL_LUMINANCE8_ALPHA8
#define GL_LUMINANCE8_ALPHA8 0x8045
#endif
#ifndef GL_LUMINANCE12_ALPHA4
#define GL_LUMINANCE12_ALPHA4 0x8046
#endif
#ifndef GL_LUMINANCE12_ALPHA12
#define GL_LUMINANCE12_ALPHA12 0x8047
#endif
#ifndef GL_LUMINANCE16_ALPHA16
#define GL_LUMINANCE16_ALPHA16 0x8048
#endif
#ifndef GL_INTENSITY
#define GL_INTENSITY 0x8049
#endif
#ifndef GL_INTENSITY4
#define GL_INTENSITY4 0x804A
#endif
#ifndef GL_INTENSITY8
#define GL_INTENSITY8 0x804B
#endif
#ifndef GL_INTENSITY12
#define GL_INTENSITY12 0x804C
#endif
#ifndef GL_INTENSITY16
#define GL_INTENSITY16 0x804D
#endif
#ifndef GL_RGB4
#define GL_RGB4 0x804F
#endif
#ifndef GL_RGB5
#define GL_RGB5 0x8050
#endif
#ifndef GL_RGB8
#define GL_RGB8 0x8051
#endif
#ifndef GL_RGB10
#define GL_RGB10 0x8052
#endif
#ifndef GL_RGB12
#define GL_RGB12 0x8053
#endif
#ifndef GL_RGB16
#define GL_RGB16 0x8054
#endif
#ifndef GL_RGBA2
#define GL_RGBA2 0x8055
#endif
#ifndef GL_RGBA4
#define GL_RGBA4 0x8056
#endif
#ifndef GL_RGB5_A1
#define GL_RGB5_A1 0x8057
#endif
#ifndef GL_RGBA8
#define GL_RGBA8 0x8058
#endif
#ifndef GL_RGB10_A2
#define GL_RGB10_A2 0x8059
#endif
#ifndef GL_RGBA12
#define GL_RGBA12 0x805A
#endif
#ifndef GL_RGBA16
#define GL_RGBA16 0x805B
#endif
#ifndef GL_V2F
#define GL_V2F 0x2A20
#endif
#ifndef GL_V3F
#define GL_V3F 0x2A21
#endif
#ifndef GL_C4UB_V2F
#define GL_C4UB_V2F 0x2A22
#endif
#ifndef GL_C4UB_V3F
#define GL_C4UB_V3F 0x2A23
#endif
#ifndef GL_C3F_V3F
#define GL_C3F_V3F 0x2A24
#endif
#ifndef GL_N3F_V3F
#define GL_N3F_V3F 0x2A25
#endif
#ifndef GL_C4F_N3F_V3F
#define GL_C4F_N3F_V3F 0x2A26
#endif
#ifndef GL_T2F_V3F
#define GL_T2F_V3F 0x2A27
#endif
#ifndef GL_T4F_V4F
#define GL_T4F_V4F 0x2A28
#endif
#ifndef GL_T2F_C4UB_V3F
#define GL_T2F_C4UB_V3F 0x2A29
#endif
#ifndef GL_T2F_C3F_V3F
#define GL_T2F_C3F_V3F 0x2A2A
#endif
#ifndef GL_T2F_N3F_V3F
#define GL_T2F_N3F_V3F 0x2A2B
#endif
#ifndef GL_T2F_C4F_N3F_V3F
#define GL_T2F_C4F_N3F_V3F 0x2A2C
#endif
#ifndef GL_T4F_C4F_N3F_V4F
#define GL_T4F_C4F_N3F_V4F 0x2A2D
#endif
#ifndef GL_CLIP_PLANE0
#define GL_CLIP_PLANE0 0x3000
#endif
#ifndef GL_CLIP_PLANE1
#define GL_CLIP_PLANE1 0x3001
#endif
#ifndef GL_CLIP_PLANE2
#define GL_CLIP_PLANE2 0x3002
#endif
#ifndef GL_CLIP_PLANE3
#define GL_CLIP_PLANE3 0x3003
#endif
#ifndef GL_CLIP_PLANE4
#define GL_CLIP_PLANE4 0x3004
#endif
#ifndef GL_CLIP_PLANE5
#define GL_CLIP_PLANE5 0x3005
#endif
#ifndef GL_LIGHT0
#define GL_LIGHT0 0x4000
#endif
#ifndef GL_LIGHT1
#define GL_LIGHT1 0x4001
#endif
#ifndef GL_LIGHT2
#define GL_LIGHT2 0x4002
#endif
#ifndef GL_LIGHT3
#define GL_LIGHT3 0x4003
#endif
#ifndef GL_LIGHT4
#define GL_LIGHT4 0x4004
#endif
#ifndef GL_LIGHT5
#define GL_LIGHT5 0x4005
#endif
#ifndef GL_LIGHT6
#define GL_LIGHT6 0x4006
#endif
#ifndef GL_LIGHT7
#define GL_LIGHT7 0x4007
#endif
#ifndef GL_ABGR_EXT
#define GL_ABGR_EXT 0x8000
#endif
#ifndef GL_CONSTANT_COLOR
#define GL_CONSTANT_COLOR 0x8001
#endif
#ifndef GL_CONSTANT_COLOR_EXT
#define GL_CONSTANT_COLOR_EXT 0x8001
#endif
#ifndef GL_ONE_MINUS_CONSTANT_COLOR
#define GL_ONE_MINUS_CONSTANT_COLOR 0x8002
#endif
#ifndef GL_ONE_MINUS_CONSTANT_COLOR_EXT
#define GL_ONE_MINUS_CONSTANT_COLOR_EXT 0x8002
#endif
#ifndef GL_CONSTANT_ALPHA
#define GL_CONSTANT_ALPHA 0x8003
#endif
#ifndef GL_CONSTANT_ALPHA_EXT
#define GL_CONSTANT_ALPHA_EXT 0x8003
#endif
#ifndef GL_ONE_MINUS_CONSTANT_ALPHA
#define GL_ONE_MINUS_CONSTANT_ALPHA 0x8004
#endif
#ifndef GL_ONE_MINUS_CONSTANT_ALPHA_EXT
#define GL_ONE_MINUS_CONSTANT_ALPHA_EXT 0x8004
#endif
#ifndef GL_BLEND_COLOR
#define GL_BLEND_COLOR 0x8005
#endif
#ifndef GL_BLEND_COLOR_EXT
#define GL_BLEND_COLOR_EXT 0x8005
#endif
#ifndef GL_FUNC_ADD
#define GL_FUNC_ADD 0x8006
#endif
#ifndef GL_FUNC_ADD_EXT
#define GL_FUNC_ADD_EXT 0x8006
#endif
#ifndef GL_MIN
#define GL_MIN 0x8007
#endif
#ifndef GL_MIN_EXT
#define GL_MIN_EXT 0x8007
#endif
#ifndef GL_MAX
#define GL_MAX 0x8008
#endif
#ifndef GL_MAX_EXT
#define GL_MAX_EXT 0x8008
#endif
#ifndef GL_BLEND_EQUATION
#define GL_BLEND_EQUATION 0x8009
#endif
#ifndef GL_BLEND_EQUATION_EXT
#define GL_BLEND_EQUATION_EXT 0x8009
#endif
#ifndef GL_FUNC_SUBTRACT
#define GL_FUNC_SUBTRACT 0x800A
#endif
#ifndef GL_FUNC_SUBTRACT_EXT
#define GL_FUNC_SUBTRACT_EXT 0x800A
#endif
#ifndef GL_FUNC_REVERSE_SUBTRACT
#define GL_FUNC_REVERSE_SUBTRACT 0x800B
#endif
#ifndef GL_FUNC_REVERSE_SUBTRACT_EXT
#define GL_FUNC_REVERSE_SUBTRACT_EXT 0x800B
#endif
#ifndef GL_CMYK_EXT
#define GL_CMYK_EXT 0x800C
#endif
#ifndef GL_CMYKA_EXT
#define GL_CMYKA_EXT 0x800D
#endif
#ifndef GL_PACK_CMYK_HINT_EXT
#define GL_PACK_CMYK_HINT_EXT 0x800E
#endif
#ifndef GL_UNPACK_CMYK_HINT_EXT
#define GL_UNPACK_CMYK_HINT_EXT 0x800F
#endif
#ifndef GL_CONVOLUTION_1D
#define GL_CONVOLUTION_1D 0x8010
#endif
#ifndef GL_CONVOLUTION_1D_EXT
#define GL_CONVOLUTION_1D_EXT 0x8010
#endif
#ifndef GL_CONVOLUTION_2D
#define GL_CONVOLUTION_2D 0x8011
#endif
#ifndef GL_CONVOLUTION_2D_EXT
#define GL_CONVOLUTION_2D_EXT 0x8011
#endif
#ifndef GL_SEPARABLE_2D
#define GL_SEPARABLE_2D 0x8012
#endif
#ifndef GL_SEPARABLE_2D_EXT
#define GL_SEPARABLE_2D_EXT 0x8012
#endif
#ifndef GL_CONVOLUTION_BORDER_MODE
#define GL_CONVOLUTION_BORDER_MODE 0x8013
#endif
#ifndef GL_CONVOLUTION_BORDER_MODE_EXT
#define GL_CONVOLUTION_BORDER_MODE_EXT 0x8013
#endif
#ifndef GL_CONVOLUTION_FILTER_SCALE
#define GL_CONVOLUTION_FILTER_SCALE 0x8014
#endif
#ifndef GL_CONVOLUTION_FILTER_SCALE_EXT
#define GL_CONVOLUTION_FILTER_SCALE_EXT 0x8014
#endif
#ifndef GL_CONVOLUTION_FILTER_BIAS
#define GL_CONVOLUTION_FILTER_BIAS 0x8015
#endif
#ifndef GL_CONVOLUTION_FILTER_BIAS_EXT
#define GL_CONVOLUTION_FILTER_BIAS_EXT 0x8015
#endif
#ifndef GL_REDUCE
#define GL_REDUCE 0x8016
#endif
#ifndef GL_REDUCE_EXT
#define GL_REDUCE_EXT 0x8016
#endif
#ifndef GL_CONVOLUTION_FORMAT
#define GL_CONVOLUTION_FORMAT 0x8017
#endif
#ifndef GL_CONVOLUTION_FORMAT_EXT
#define GL_CONVOLUTION_FORMAT_EXT 0x8017
#endif
#ifndef GL_CONVOLUTION_WIDTH
#define GL_CONVOLUTION_WIDTH 0x8018
#endif
#ifndef GL_CONVOLUTION_WIDTH_EXT
#define GL_CONVOLUTION_WIDTH_EXT 0x8018
#endif
#ifndef GL_CONVOLUTION_HEIGHT
#define GL_CONVOLUTION_HEIGHT 0x8019
#endif
#ifndef GL_CONVOLUTION_HEIGHT_EXT
#define GL_CONVOLUTION_HEIGHT_EXT 0x8019
#endif
#ifndef GL_MAX_CONVOLUTION_WIDTH
#define GL_MAX_CONVOLUTION_WIDTH 0x801A
#endif
#ifndef GL_MAX_CONVOLUTION_WIDTH_EXT
#define GL_MAX_CONVOLUTION_WIDTH_EXT 0x801A
#endif
#ifndef GL_MAX_CONVOLUTION_HEIGHT
#define GL_MAX_CONVOLUTION_HEIGHT 0x801B
#endif
#ifndef GL_MAX_CONVOLUTION_HEIGHT_EXT
#define GL_MAX_CONVOLUTION_HEIGHT_EXT 0x801B
#endif
#ifndef GL_POST_CONVOLUTION_RED_SCALE
#define GL_POST_CONVOLUTION_RED_SCALE 0x801C
#endif
#ifndef GL_POST_CONVOLUTION_RED_SCALE_EXT
#define GL_POST_CONVOLUTION_RED_SCALE_EXT 0x801C
#endif
#ifndef GL_POST_CONVOLUTION_GREEN_SCALE
#define GL_POST_CONVOLUTION_GREEN_SCALE 0x801D
#endif
#ifndef GL_POST_CONVOLUTION_GREEN_SCALE_EXT
#define GL_POST_CONVOLUTION_GREEN_SCALE_EXT 0x801D
#endif
#ifndef GL_POST_CONVOLUTION_BLUE_SCALE
#define GL_POST_CONVOLUTION_BLUE_SCALE 0x801E
#endif
#ifndef GL_POST_CONVOLUTION_BLUE_SCALE_EXT
#define GL_POST_CONVOLUTION_BLUE_SCALE_EXT 0x801E
#endif
#ifndef GL_POST_CONVOLUTION_ALPHA_SCALE
#define GL_POST_CONVOLUTION_ALPHA_SCALE 0x801F
#endif
#ifndef GL_POST_CONVOLUTION_ALPHA_SCALE_EXT
#define GL_POST_CONVOLUTION_ALPHA_SCALE_EXT 0x801F
#endif
#ifndef GL_POST_CONVOLUTION_RED_BIAS
#define GL_POST_CONVOLUTION_RED_BIAS 0x8020
#endif
#ifndef GL_POST_CONVOLUTION_RED_BIAS_EXT
#define GL_POST_CONVOLUTION_RED_BIAS_EXT 0x8020
#endif
#ifndef GL_POST_CONVOLUTION_GREEN_BIAS
#define GL_POST_CONVOLUTION_GREEN_BIAS 0x8021
#endif
#ifndef GL_POST_CONVOLUTION_GREEN_BIAS_EXT
#define GL_POST_CONVOLUTION_GREEN_BIAS_EXT 0x8021
#endif
#ifndef GL_POST_CONVOLUTION_BLUE_BIAS
#define GL_POST_CONVOLUTION_BLUE_BIAS 0x8022
#endif
#ifndef GL_POST_CONVOLUTION_BLUE_BIAS_EXT
#define GL_POST_CONVOLUTION_BLUE_BIAS_EXT 0x8022
#endif
#ifndef GL_POST_CONVOLUTION_ALPHA_BIAS
#define GL_POST_CONVOLUTION_ALPHA_BIAS 0x8023
#endif
#ifndef GL_POST_CONVOLUTION_ALPHA_BIAS_EXT
#define GL_POST_CONVOLUTION_ALPHA_BIAS_EXT 0x8023
#endif
#ifndef GL_HISTOGRAM
#define GL_HISTOGRAM 0x8024
#endif
#ifndef GL_HISTOGRAM_EXT
#define GL_HISTOGRAM_EXT 0x8024
#endif
#ifndef GL_PROXY_HISTOGRAM
#define GL_PROXY_HISTOGRAM 0x8025
#endif
#ifndef GL_PROXY_HISTOGRAM_EXT
#define GL_PROXY_HISTOGRAM_EXT 0x8025
#endif
#ifndef GL_HISTOGRAM_WIDTH
#define GL_HISTOGRAM_WIDTH 0x8026
#endif
#ifndef GL_HISTOGRAM_WIDTH_EXT
#define GL_HISTOGRAM_WIDTH_EXT 0x8026
#endif
#ifndef GL_HISTOGRAM_FORMAT
#define GL_HISTOGRAM_FORMAT 0x8027
#endif
#ifndef GL_HISTOGRAM_FORMAT_EXT
#define GL_HISTOGRAM_FORMAT_EXT 0x8027
#endif
#ifndef GL_HISTOGRAM_RED_SIZE
#define GL_HISTOGRAM_RED_SIZE 0x8028
#endif
#ifndef GL_HISTOGRAM_RED_SIZE_EXT
#define GL_HISTOGRAM_RED_SIZE_EXT 0x8028
#endif
#ifndef GL_HISTOGRAM_GREEN_SIZE
#define GL_HISTOGRAM_GREEN_SIZE 0x8029
#endif
#ifndef GL_HISTOGRAM_GREEN_SIZE_EXT
#define GL_HISTOGRAM_GREEN_SIZE_EXT 0x8029
#endif
#ifndef GL_HISTOGRAM_BLUE_SIZE
#define GL_HISTOGRAM_BLUE_SIZE 0x802A
#endif
#ifndef GL_HISTOGRAM_BLUE_SIZE_EXT
#define GL_HISTOGRAM_BLUE_SIZE_EXT 0x802A
#endif
#ifndef GL_HISTOGRAM_ALPHA_SIZE
#define GL_HISTOGRAM_ALPHA_SIZE 0x802B
#endif
#ifndef GL_HISTOGRAM_ALPHA_SIZE_EXT
#define GL_HISTOGRAM_ALPHA_SIZE_EXT 0x802B
#endif
#ifndef GL_HISTOGRAM_LUMINANCE_SIZE
#define GL_HISTOGRAM_LUMINANCE_SIZE 0x802C
#endif
#ifndef GL_HISTOGRAM_LUMINANCE_SIZE_EXT
#define GL_HISTOGRAM_LUMINANCE_SIZE_EXT 0x802C
#endif
#ifndef GL_HISTOGRAM_SINK
#define GL_HISTOGRAM_SINK 0x802D
#endif
#ifndef GL_HISTOGRAM_SINK_EXT
#define GL_HISTOGRAM_SINK_EXT 0x802D
#endif
#ifndef GL_MINMAX
#define GL_MINMAX 0x802E
#endif
#ifndef GL_MINMAX_EXT
#define GL_MINMAX_EXT 0x802E
#endif
#ifndef GL_MINMAX_FORMAT
#define GL_MINMAX_FORMAT 0x802F
#endif
#ifndef GL_MINMAX_FORMAT_EXT
#define GL_MINMAX_FORMAT_EXT 0x802F
#endif
#ifndef GL_MINMAX_SINK
#define GL_MINMAX_SINK 0x8030
#endif
#ifndef GL_MINMAX_SINK_EXT
#define GL_MINMAX_SINK_EXT 0x8030
#endif
#ifndef GL_TABLE_TOO_LARGE
#define GL_TABLE_TOO_LARGE 0x8031
#endif
#ifndef GL_TABLE_TOO_LARGE_EXT
#define GL_TABLE_TOO_LARGE_EXT 0x8031
#endif
#ifndef GL_UNSIGNED_BYTE_3_3_2
#define GL_UNSIGNED_BYTE_3_3_2 0x8032
#endif
#ifndef GL_UNSIGNED_BYTE_3_3_2_EXT
#define GL_UNSIGNED_BYTE_3_3_2_EXT 0x8032
#endif
#ifndef GL_UNSIGNED_SHORT_4_4_4_4
#define GL_UNSIGNED_SHORT_4_4_4_4 0x8033
#endif
#ifndef GL_UNSIGNED_SHORT_4_4_4_4_EXT
#define GL_UNSIGNED_SHORT_4_4_4_4_EXT 0x8033
#endif
#ifndef GL_UNSIGNED_SHORT_5_5_5_1
#define GL_UNSIGNED_SHORT_5_5_5_1 0x8034
#endif
#ifndef GL_UNSIGNED_SHORT_5_5_5_1_EXT
#define GL_UNSIGNED_SHORT_5_5_5_1_EXT 0x8034
#endif
#ifndef GL_UNSIGNED_INT_8_8_8_8
#define GL_UNSIGNED_INT_8_8_8_8 0x8035
#endif
#ifndef GL_UNSIGNED_INT_8_8_8_8_EXT
#define GL_UNSIGNED_INT_8_8_8_8_EXT 0x8035
#endif
#ifndef GL_UNSIGNED_INT_10_10_10_2
#define GL_UNSIGNED_INT_10_10_10_2 0x8036
#endif
#ifndef GL_UNSIGNED_INT_10_10_10_2_EXT
#define GL_UNSIGNED_INT_10_10_10_2_EXT 0x8036
#endif
#ifndef GL_UNSIGNED_BYTE_2_3_3_REV
#define GL_UNSIGNED_BYTE_2_3_3_REV 0x8362
#endif
#ifndef GL_UNSIGNED_BYTE_2_3_3_REV_EXT
#define GL_UNSIGNED_BYTE_2_3_3_REV_EXT 0x8362
#endif
#ifndef GL_UNSIGNED_SHORT_5_6_5
#define GL_UNSIGNED_SHORT_5_6_5 0x8363
#endif
#ifndef GL_UNSIGNED_SHORT_5_6_5_EXT
#define GL_UNSIGNED_SHORT_5_6_5_EXT 0x8363
#endif
#ifndef GL_UNSIGNED_SHORT_5_6_5_REV
#define GL_UNSIGNED_SHORT_5_6_5_REV 0x8364
#endif
#ifndef GL_UNSIGNED_SHORT_5_6_5_REV_EXT
#define GL_UNSIGNED_SHORT_5_6_5_REV_EXT 0x8364
#endif
#ifndef GL_UNSIGNED_SHORT_4_4_4_4_REV
#define GL_UNSIGNED_SHORT_4_4_4_4_REV 0x8365
#endif
#ifndef GL_UNSIGNED_SHORT_4_4_4_4_REV_EXT
#define GL_UNSIGNED_SHORT_4_4_4_4_REV_EXT 0x8365
#endif
#ifndef GL_UNSIGNED_SHORT_1_5_5_5_REV
#define GL_UNSIGNED_SHORT_1_5_5_5_REV 0x8366
#endif
#ifndef GL_UNSIGNED_SHORT_1_5_5_5_REV_EXT
#define GL_UNSIGNED_SHORT_1_5_5_5_REV_EXT 0x8366
#endif
#ifndef GL_UNSIGNED_INT_8_8_8_8_REV
#define GL_UNSIGNED_INT_8_8_8_8_REV 0x8367
#endif
#ifndef GL_UNSIGNED_INT_8_8_8_8_REV_EXT
#define GL_UNSIGNED_INT_8_8_8_8_REV_EXT 0x8367
#endif
#ifndef GL_UNSIGNED_INT_2_10_10_10_REV
#define GL_UNSIGNED_INT_2_10_10_10_REV 0x8368
#endif
#ifndef GL_UNSIGNED_INT_2_10_10_10_REV_EXT
#define GL_UNSIGNED_INT_2_10_10_10_REV_EXT 0x8368
#endif
#ifndef GL_POLYGON_OFFSET_EXT
#define GL_POLYGON_OFFSET_EXT 0x8037
#endif
#ifndef GL_POLYGON_OFFSET_FACTOR_EXT
#define GL_POLYGON_OFFSET_FACTOR_EXT 0x8038
#endif
#ifndef GL_POLYGON_OFFSET_BIAS_EXT
#define GL_POLYGON_OFFSET_BIAS_EXT 0x8039
#endif
#ifndef GL_RESCALE_NORMAL
#define GL_RESCALE_NORMAL 0x803A
#endif
#ifndef GL_RESCALE_NORMAL_EXT
#define GL_RESCALE_NORMAL_EXT 0x803A
#endif
#ifndef GL_ALPHA4_EXT
#define GL_ALPHA4_EXT 0x803B
#endif
#ifndef GL_ALPHA8_EXT
#define GL_ALPHA8_EXT 0x803C
#endif
#ifndef GL_ALPHA12_EXT
#define GL_ALPHA12_EXT 0x803D
#endif
#ifndef GL_ALPHA16_EXT
#define GL_ALPHA16_EXT 0x803E
#endif
#ifndef GL_LUMINANCE4_EXT
#define GL_LUMINANCE4_EXT 0x803F
#endif
#ifndef GL_LUMINANCE8_EXT
#define GL_LUMINANCE8_EXT 0x8040
#endif
#ifndef GL_LUMINANCE12_EXT
#define GL_LUMINANCE12_EXT 0x8041
#endif
#ifndef GL_LUMINANCE16_EXT
#define GL_LUMINANCE16_EXT 0x8042
#endif
#ifndef GL_LUMINANCE4_ALPHA4_EXT
#define GL_LUMINANCE4_ALPHA4_EXT 0x8043
#endif
#ifndef GL_LUMINANCE6_ALPHA2_EXT
#define GL_LUMINANCE6_ALPHA2_EXT 0x8044
#endif
#ifndef GL_LUMINANCE8_ALPHA8_EXT
#define GL_LUMINANCE8_ALPHA8_EXT 0x8045
#endif
#ifndef GL_LUMINANCE12_ALPHA4_EXT
#define GL_LUMINANCE12_ALPHA4_EXT 0x8046
#endif
#ifndef GL_LUMINANCE12_ALPHA12_EXT
#define GL_LUMINANCE12_ALPHA12_EXT 0x8047
#endif
#ifndef GL_LUMINANCE16_ALPHA16_EXT
#define GL_LUMINANCE16_ALPHA16_EXT 0x8048
#endif
#ifndef GL_INTENSITY_EXT
#define GL_INTENSITY_EXT 0x8049
#endif
#ifndef GL_INTENSITY4_EXT
#define GL_INTENSITY4_EXT 0x804A
#endif
#ifndef GL_INTENSITY8_EXT
#define GL_INTENSITY8_EXT 0x804B
#endif
#ifndef GL_INTENSITY12_EXT
#define GL_INTENSITY12_EXT 0x804C
#endif
#ifndef GL_INTENSITY16_EXT
#define GL_INTENSITY16_EXT 0x804D
#endif
#ifndef GL_RGB2_EXT
#define GL_RGB2_EXT 0x804E
#endif
#ifndef GL_RGB4_EXT
#define GL_RGB4_EXT 0x804F
#endif
#ifndef GL_RGB5_EXT
#define GL_RGB5_EXT 0x8050
#endif
#ifndef GL_RGB8_EXT
#define GL_RGB8_EXT 0x8051
#endif
#ifndef GL_RGB10_EXT
#define GL_RGB10_EXT 0x8052
#endif
#ifndef GL_RGB12_EXT
#define GL_RGB12_EXT 0x8053
#endif
#ifndef GL_RGB16_EXT
#define GL_RGB16_EXT 0x8054
#endif
#ifndef GL_RGBA2_EXT
#define GL_RGBA2_EXT 0x8055
#endif
#ifndef GL_RGBA4_EXT
#define GL_RGBA4_EXT 0x8056
#endif
#ifndef GL_RGB5_A1_EXT
#define GL_RGB5_A1_EXT 0x8057
#endif
#ifndef GL_RGBA8_EXT
#define GL_RGBA8_EXT 0x8058
#endif
#ifndef GL_RGB10_A2_EXT
#define GL_RGB10_A2_EXT 0x8059
#endif
#ifndef GL_RGBA12_EXT
#define GL_RGBA12_EXT 0x805A
#endif
#ifndef GL_RGBA16_EXT
#define GL_RGBA16_EXT 0x805B
#endif
#ifndef GL_TEXTURE_RED_SIZE_EXT
#define GL_TEXTURE_RED_SIZE_EXT 0x805C
#endif
#ifndef GL_TEXTURE_GREEN_SIZE_EXT
#define GL_TEXTURE_GREEN_SIZE_EXT 0x805D
#endif
#ifndef GL_TEXTURE_BLUE_SIZE_EXT
#define GL_TEXTURE_BLUE_SIZE_EXT 0x805E
#endif
#ifndef GL_TEXTURE_ALPHA_SIZE_EXT
#define GL_TEXTURE_ALPHA_SIZE_EXT 0x805F
#endif
#ifndef GL_TEXTURE_LUMINANCE_SIZE_EXT
#define GL_TEXTURE_LUMINANCE_SIZE_EXT 0x8060
#endif
#ifndef GL_TEXTURE_INTENSITY_SIZE_EXT
#define GL_TEXTURE_INTENSITY_SIZE_EXT 0x8061
#endif
#ifndef GL_REPLACE_EXT
#define GL_REPLACE_EXT 0x8062
#endif
#ifndef GL_PROXY_TEXTURE_1D_EXT
#define GL_PROXY_TEXTURE_1D_EXT 0x8063
#endif
#ifndef GL_PROXY_TEXTURE_2D_EXT
#define GL_PROXY_TEXTURE_2D_EXT 0x8064
#endif
#ifndef GL_TEXTURE_TOO_LARGE_EXT
#define GL_TEXTURE_TOO_LARGE_EXT 0x8065
#endif
#ifndef GL_TEXTURE_PRIORITY_EXT
#define GL_TEXTURE_PRIORITY_EXT 0x8066
#endif
#ifndef GL_TEXTURE_RESIDENT_EXT
#define GL_TEXTURE_RESIDENT_EXT 0x8067
#endif
#ifndef GL_TEXTURE_1D_BINDING_EXT
#define GL_TEXTURE_1D_BINDING_EXT 0x8068
#endif
#ifndef GL_TEXTURE_2D_BINDING_EXT
#define GL_TEXTURE_2D_BINDING_EXT 0x8069
#endif
#ifndef GL_TEXTURE_3D_BINDING_EXT
#define GL_TEXTURE_3D_BINDING_EXT 0x806A
#endif
#ifndef GL_PACK_SKIP_IMAGES
#define GL_PACK_SKIP_IMAGES 0x806B
#endif
#ifndef GL_PACK_SKIP_IMAGES_EXT
#define GL_PACK_SKIP_IMAGES_EXT 0x806B
#endif
#ifndef GL_PACK_IMAGE_HEIGHT
#define GL_PACK_IMAGE_HEIGHT 0x806C
#endif
#ifndef GL_PACK_IMAGE_HEIGHT_EXT
#define GL_PACK_IMAGE_HEIGHT_EXT 0x806C
#endif
#ifndef GL_UNPACK_SKIP_IMAGES
#define GL_UNPACK_SKIP_IMAGES 0x806D
#endif
#ifndef GL_UNPACK_SKIP_IMAGES_EXT
#define GL_UNPACK_SKIP_IMAGES_EXT 0x806D
#endif
#ifndef GL_UNPACK_IMAGE_HEIGHT
#define GL_UNPACK_IMAGE_HEIGHT 0x806E
#endif
#ifndef GL_UNPACK_IMAGE_HEIGHT_EXT
#define GL_UNPACK_IMAGE_HEIGHT_EXT 0x806E
#endif
#ifndef GL_TEXTURE_3D
#define GL_TEXTURE_3D 0x806F
#endif
#ifndef GL_TEXTURE_3D_EXT
#define GL_TEXTURE_3D_EXT 0x806F
#endif
#ifndef GL_PROXY_TEXTURE_3D
#define GL_PROXY_TEXTURE_3D 0x8070
#endif
#ifndef GL_PROXY_TEXTURE_3D_EXT
#define GL_PROXY_TEXTURE_3D_EXT 0x8070
#endif
#ifndef GL_TEXTURE_DEPTH
#define GL_TEXTURE_DEPTH 0x8071
#endif
#ifndef GL_TEXTURE_DEPTH_EXT
#define GL_TEXTURE_DEPTH_EXT 0x8071
#endif
#ifndef GL_TEXTURE_WRAP_R
#define GL_TEXTURE_WRAP_R 0x8072
#endif
#ifndef GL_TEXTURE_WRAP_R_EXT
#define GL_TEXTURE_WRAP_R_EXT 0x8072
#endif
#ifndef GL_MAX_3D_TEXTURE_SIZE
#define GL_MAX_3D_TEXTURE_SIZE 0x8073
#endif
#ifndef GL_MAX_3D_TEXTURE_SIZE_EXT
#define GL_MAX_3D_TEXTURE_SIZE_EXT 0x8073
#endif
#ifndef GL_VERTEX_ARRAY_EXT
#define GL_VERTEX_ARRAY_EXT 0x8074
#endif
#ifndef GL_NORMAL_ARRAY_EXT
#define GL_NORMAL_ARRAY_EXT 0x8075
#endif
#ifndef GL_COLOR_ARRAY_EXT
#define GL_COLOR_ARRAY_EXT 0x8076
#endif
#ifndef GL_INDEX_ARRAY_EXT
#define GL_INDEX_ARRAY_EXT 0x8077
#endif
#ifndef GL_TEXTURE_COORD_ARRAY_EXT
#define GL_TEXTURE_COORD_ARRAY_EXT 0x8078
#endif
#ifndef GL_EDGE_FLAG_ARRAY_EXT
#define GL_EDGE_FLAG_ARRAY_EXT 0x8079
#endif
#ifndef GL_VERTEX_ARRAY_SIZE_EXT
#define GL_VERTEX_ARRAY_SIZE_EXT 0x807A
#endif
#ifndef GL_VERTEX_ARRAY_TYPE_EXT
#define GL_VERTEX_ARRAY_TYPE_EXT 0x807B
#endif
#ifndef GL_VERTEX_ARRAY_STRIDE_EXT
#define GL_VERTEX_ARRAY_STRIDE_EXT 0x807C
#endif
#ifndef GL_VERTEX_ARRAY_COUNT_EXT
#define GL_VERTEX_ARRAY_COUNT_EXT 0x807D
#endif
#ifndef GL_NORMAL_ARRAY_TYPE_EXT
#define GL_NORMAL_ARRAY_TYPE_EXT 0x807E
#endif
#ifndef GL_NORMAL_ARRAY_STRIDE_EXT
#define GL_NORMAL_ARRAY_STRIDE_EXT 0x807F
#endif
#ifndef GL_NORMAL_ARRAY_COUNT_EXT
#define GL_NORMAL_ARRAY_COUNT_EXT 0x8080
#endif
#ifndef GL_COLOR_ARRAY_SIZE_EXT
#define GL_COLOR_ARRAY_SIZE_EXT 0x8081
#endif
#ifndef GL_COLOR_ARRAY_TYPE_EXT
#define GL_COLOR_ARRAY_TYPE_EXT 0x8082
#endif
#ifndef GL_COLOR_ARRAY_STRIDE_EXT
#define GL_COLOR_ARRAY_STRIDE_EXT 0x8083
#endif
#ifndef GL_COLOR_ARRAY_COUNT_EXT
#define GL_COLOR_ARRAY_COUNT_EXT 0x8084
#endif
#ifndef GL_INDEX_ARRAY_TYPE_EXT
#define GL_INDEX_ARRAY_TYPE_EXT 0x8085
#endif
#ifndef GL_INDEX_ARRAY_STRIDE_EXT
#define GL_INDEX_ARRAY_STRIDE_EXT 0x8086
#endif
#ifndef GL_INDEX_ARRAY_COUNT_EXT
#define GL_INDEX_ARRAY_COUNT_EXT 0x8087
#endif
#ifndef GL_TEXTURE_COORD_ARRAY_SIZE_EXT
#define GL_TEXTURE_COORD_ARRAY_SIZE_EXT 0x8088
#endif
#ifndef GL_TEXTURE_COORD_ARRAY_TYPE_EXT
#define GL_TEXTURE_COORD_ARRAY_TYPE_EXT 0x8089
#endif
#ifndef GL_TEXTURE_COORD_ARRAY_STRIDE_EXT
#define GL_TEXTURE_COORD_ARRAY_STRIDE_EXT 0x808A
#endif
#ifndef GL_TEXTURE_COORD_ARRAY_COUNT_EXT
#define GL_TEXTURE_COORD_ARRAY_COUNT_EXT 0x808B
#endif
#ifndef GL_EDGE_FLAG_ARRAY_STRIDE_EXT
#define GL_EDGE_FLAG_ARRAY_STRIDE_EXT 0x808C
#endif
#ifndef GL_EDGE_FLAG_ARRAY_COUNT_EXT
#define GL_EDGE_FLAG_ARRAY_COUNT_EXT 0x808D
#endif
#ifndef GL_VERTEX_ARRAY_POINTER_EXT
#define GL_VERTEX_ARRAY_POINTER_EXT 0x808E
#endif
#ifndef GL_NORMAL_ARRAY_POINTER_EXT
#define GL_NORMAL_ARRAY_POINTER_EXT 0x808F
#endif
#ifndef GL_COLOR_ARRAY_POINTER_EXT
#define GL_COLOR_ARRAY_POINTER_EXT 0x8090
#endif
#ifndef GL_INDEX_ARRAY_POINTER_EXT
#define GL_INDEX_ARRAY_POINTER_EXT 0x8091
#endif
#ifndef GL_TEXTURE_COORD_ARRAY_POINTER_EXT
#define GL_TEXTURE_COORD_ARRAY_POINTER_EXT 0x8092
#endif
#ifndef GL_EDGE_FLAG_ARRAY_POINTER_EXT
#define GL_EDGE_FLAG_ARRAY_POINTER_EXT 0x8093
#endif
#ifndef GL_INTERLACE_SGIX
#define GL_INTERLACE_SGIX 0x8094
#endif
#ifndef GL_DETAIL_TEXTURE_2D_SGIS
#define GL_DETAIL_TEXTURE_2D_SGIS 0x8095
#endif
#ifndef GL_DETAIL_TEXTURE_2D_BINDING_SGIS
#define GL_DETAIL_TEXTURE_2D_BINDING_SGIS 0x8096
#endif
#ifndef GL_LINEAR_DETAIL_SGIS
#define GL_LINEAR_DETAIL_SGIS 0x8097
#endif
#ifndef GL_LINEAR_DETAIL_ALPHA_SGIS
#define GL_LINEAR_DETAIL_ALPHA_SGIS 0x8098
#endif
#ifndef GL_LINEAR_DETAIL_COLOR_SGIS
#define GL_LINEAR_DETAIL_COLOR_SGIS 0x8099
#endif
#ifndef GL_DETAIL_TEXTURE_LEVEL_SGIS
#define GL_DETAIL_TEXTURE_LEVEL_SGIS 0x809A
#endif
#ifndef GL_DETAIL_TEXTURE_MODE_SGIS
#define GL_DETAIL_TEXTURE_MODE_SGIS 0x809B
#endif
#ifndef GL_DETAIL_TEXTURE_FUNC_POINTS_SGIS
#define GL_DETAIL_TEXTURE_FUNC_POINTS_SGIS 0x809C
#endif
#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE 0x809D
#endif
#ifndef GL_MULTISAMPLE_ARB
#define GL_MULTISAMPLE_ARB 0x809D
#endif
#ifndef GL_SAMPLE_ALPHA_TO_COVERAGE
#define GL_SAMPLE_ALPHA_TO_COVERAGE 0x809E
#endif
#ifndef GL_SAMPLE_ALPHA_TO_COVERAGE_ARB
#define GL_SAMPLE_ALPHA_TO_COVERAGE_ARB 0x809E
#endif
#ifndef GL_SAMPLE_ALPHA_TO_ONE
#define GL_SAMPLE_ALPHA_TO_ONE 0x809F
#endif
#ifndef GL_SAMPLE_ALPHA_TO_ONE_ARB
#define GL_SAMPLE_ALPHA_TO_ONE_ARB 0x809F
#endif
#ifndef GL_SAMPLE_COVERAGE
#define GL_SAMPLE_COVERAGE 0x80A0
#endif
#ifndef GL_SAMPLE_COVERAGE_ARB
#define GL_SAMPLE_COVERAGE_ARB 0x80A0
#endif
#ifndef GL_SAMPLE_BUFFERS
#define GL_SAMPLE_BUFFERS 0x80A8
#endif
#ifndef GL_SAMPLE_BUFFERS_ARB
#define GL_SAMPLE_BUFFERS_ARB 0x80A8
#endif
#ifndef GL_SAMPLES
#define GL_SAMPLES 0x80A9
#endif
#ifndef GL_SAMPLES_ARB
#define GL_SAMPLES_ARB 0x80A9
#endif
#ifndef GL_SAMPLE_COVERAGE_VALUE
#define GL_SAMPLE_COVERAGE_VALUE 0x80AA
#endif
#ifndef GL_SAMPLE_COVERAGE_VALUE_ARB
#define GL_SAMPLE_COVERAGE_VALUE_ARB 0x80AA
#endif
#ifndef GL_SAMPLE_COVERAGE_INVERT
#define GL_SAMPLE_COVERAGE_INVERT 0x80AB
#endif
#ifndef GL_SAMPLE_COVERAGE_INVERT_ARB
#define GL_SAMPLE_COVERAGE_INVERT_ARB 0x80AB
#endif
#ifndef GL_MULTISAMPLE_SGIS
#define GL_MULTISAMPLE_SGIS 0x809D
#endif
#ifndef GL_SAMPLE_ALPHA_TO_MASK_SGIS
#define GL_SAMPLE_ALPHA_TO_MASK_SGIS 0x809E
#endif
#ifndef GL_SAMPLE_ALPHA_TO_ONE_SGIS
#define GL_SAMPLE_ALPHA_TO_ONE_SGIS 0x809F
#endif
#ifndef GL_SAMPLE_MASK_SGIS
#define GL_SAMPLE_MASK_SGIS 0x80A0
#endif
#ifndef GL_1PASS_SGIS
#define GL_1PASS_SGIS 0x80A1
#endif
#ifndef GL_2PASS_0_SGIS
#define GL_2PASS_0_SGIS 0x80A2
#endif
#ifndef GL_2PASS_1_SGIS
#define GL_2PASS_1_SGIS 0x80A3
#endif
#ifndef GL_4PASS_0_SGIS
#define GL_4PASS_0_SGIS 0x80A4
#endif
#ifndef GL_4PASS_1_SGIS
#define GL_4PASS_1_SGIS 0x80A5
#endif
#ifndef GL_4PASS_2_SGIS
#define GL_4PASS_2_SGIS 0x80A6
#endif
#ifndef GL_4PASS_3_SGIS
#define GL_4PASS_3_SGIS 0x80A7
#endif
#ifndef GL_SAMPLE_BUFFERS_SGIS
#define GL_SAMPLE_BUFFERS_SGIS 0x80A8
#endif
#ifndef GL_SAMPLES_SGIS
#define GL_SAMPLES_SGIS 0x80A9
#endif
#ifndef GL_SAMPLE_MASK_VALUE_SGIS
#define GL_SAMPLE_MASK_VALUE_SGIS 0x80AA
#endif
#ifndef GL_SAMPLE_MASK_INVERT_SGIS
#define GL_SAMPLE_MASK_INVERT_SGIS 0x80AB
#endif
#ifndef GL_SAMPLE_PATTERN_SGIS
#define GL_SAMPLE_PATTERN_SGIS 0x80AC
#endif
#ifndef GL_LINEAR_SHARPEN_SGIS
#define GL_LINEAR_SHARPEN_SGIS 0x80AD
#endif
#ifndef GL_LINEAR_SHARPEN_ALPHA_SGIS
#define GL_LINEAR_SHARPEN_ALPHA_SGIS 0x80AE
#endif
#ifndef GL_LINEAR_SHARPEN_COLOR_SGIS
#define GL_LINEAR_SHARPEN_COLOR_SGIS 0x80AF
#endif
#ifndef GL_SHARPEN_TEXTURE_FUNC_POINTS_SGIS
#define GL_SHARPEN_TEXTURE_FUNC_POINTS_SGIS 0x80B0
#endif
#ifndef GL_COLOR_MATRIX
#define GL_COLOR_MATRIX 0x80B1
#endif
#ifndef GL_COLOR_MATRIX_SGI
#define GL_COLOR_MATRIX_SGI 0x80B1
#endif
#ifndef GL_COLOR_MATRIX_STACK_DEPTH
#define GL_COLOR_MATRIX_STACK_DEPTH 0x80B2
#endif
#ifndef GL_COLOR_MATRIX_STACK_DEPTH_SGI
#define GL_COLOR_MATRIX_STACK_DEPTH_SGI 0x80B2
#endif
#ifndef GL_MAX_COLOR_MATRIX_STACK_DEPTH
#define GL_MAX_COLOR_MATRIX_STACK_DEPTH 0x80B3
#endif
#ifndef GL_MAX_COLOR_MATRIX_STACK_DEPTH_SGI
#define GL_MAX_COLOR_MATRIX_STACK_DEPTH_SGI 0x80B3
#endif
#ifndef GL_POST_COLOR_MATRIX_RED_SCALE
#define GL_POST_COLOR_MATRIX_RED_SCALE 0x80B4
#endif
#ifndef GL_POST_COLOR_MATRIX_RED_SCALE_SGI
#define GL_POST_COLOR_MATRIX_RED_SCALE_SGI 0x80B4
#endif
#ifndef GL_POST_COLOR_MATRIX_GREEN_SCALE
#define GL_POST_COLOR_MATRIX_GREEN_SCALE 0x80B5
#endif
#ifndef GL_POST_COLOR_MATRIX_GREEN_SCALE_SGI
#define GL_POST_COLOR_MATRIX_GREEN_SCALE_SGI 0x80B5
#endif
#ifndef GL_POST_COLOR_MATRIX_BLUE_SCALE
#define GL_POST_COLOR_MATRIX_BLUE_SCALE 0x80B6
#endif
#ifndef GL_POST_COLOR_MATRIX_BLUE_SCALE_SGI
#define GL_POST_COLOR_MATRIX_BLUE_SCALE_SGI 0x80B6
#endif
#ifndef GL_POST_COLOR_MATRIX_ALPHA_SCALE
#define GL_POST_COLOR_MATRIX_ALPHA_SCALE 0x80B7
#endif
#ifndef GL_POST_COLOR_MATRIX_ALPHA_SCALE_SGI
#define GL_POST_COLOR_MATRIX_ALPHA_SCALE_SGI 0x80B7
#endif
#ifndef GL_POST_COLOR_MATRIX_RED_BIAS
#define GL_POST_COLOR_MATRIX_RED_BIAS 0x80B8
#endif
#ifndef GL_POST_COLOR_MATRIX_RED_BIAS_SGI
#define GL_POST_COLOR_MATRIX_RED_BIAS_SGI 0x80B8
#endif
#ifndef GL_POST_COLOR_MATRIX_GREEN_BIAS
#define GL_POST_COLOR_MATRIX_GREEN_BIAS 0x80B9
#endif
#ifndef GL_POST_COLOR_MATRIX_GREEN_BIAS_SGI
#define GL_POST_COLOR_MATRIX_GREEN_BIAS_SGI 0x80B9
#endif
#ifndef GL_POST_COLOR_MATRIX_BLUE_BIAS
#define GL_POST_COLOR_MATRIX_BLUE_BIAS 0x80BA
#endif
#ifndef GL_POST_COLOR_MATRIX_BLUE_BIAS_SGI
#define GL_POST_COLOR_MATRIX_BLUE_BIAS_SGI 0x80BA
#endif
#ifndef GL_POST_COLOR_MATRIX_ALPHA_BIAS
#define GL_POST_COLOR_MATRIX_ALPHA_BIAS 0x80BB
#endif
#ifndef GL_POST_COLOR_MATRIX_ALPHA_BIAS_SGI
#define GL_POST_COLOR_MATRIX_ALPHA_BIAS_SGI 0x80BB
#endif
#ifndef GL_TEXTURE_COLOR_TABLE_SGI
#define GL_TEXTURE_COLOR_TABLE_SGI 0x80BC
#endif
#ifndef GL_PROXY_TEXTURE_COLOR_TABLE_SGI
#define GL_PROXY_TEXTURE_COLOR_TABLE_SGI 0x80BD
#endif
#ifndef GL_TEXTURE_ENV_BIAS_SGIX
#define GL_TEXTURE_ENV_BIAS_SGIX 0x80BE
#endif
#ifndef GL_SHADOW_AMBIENT_SGIX
#define GL_SHADOW_AMBIENT_SGIX 0x80BF
#endif
#ifndef GL_COLOR_TABLE
#define GL_COLOR_TABLE 0x80D0
#endif
#ifndef GL_COLOR_TABLE_SGI
#define GL_COLOR_TABLE_SGI 0x80D0
#endif
#ifndef GL_POST_CONVOLUTION_COLOR_TABLE
#define GL_POST_CONVOLUTION_COLOR_TABLE 0x80D1
#endif
#ifndef GL_POST_CONVOLUTION_COLOR_TABLE_SGI
#define GL_POST_CONVOLUTION_COLOR_TABLE_SGI 0x80D1
#endif
#ifndef GL_POST_COLOR_MATRIX_COLOR_TABLE
#define GL_POST_COLOR_MATRIX_COLOR_TABLE 0x80D2
#endif
#ifndef GL_POST_COLOR_MATRIX_COLOR_TABLE_SGI
#define GL_POST_COLOR_MATRIX_COLOR_TABLE_SGI 0x80D2
#endif
#ifndef GL_PROXY_COLOR_TABLE
#define GL_PROXY_COLOR_TABLE 0x80D3
#endif
#ifndef GL_PROXY_COLOR_TABLE_SGI
#define GL_PROXY_COLOR_TABLE_SGI 0x80D3
#endif
#ifndef GL_PROXY_POST_CONVOLUTION_COLOR_TABLE
#define GL_PROXY_POST_CONVOLUTION_COLOR_TABLE 0x80D4
#endif
#ifndef GL_PROXY_POST_CONVOLUTION_COLOR_TABLE_SGI
#define GL_PROXY_POST_CONVOLUTION_COLOR_TABLE_SGI 0x80D4
#endif
#ifndef GL_PROXY_POST_COLOR_MATRIX_COLOR_TABLE
#define GL_PROXY_POST_COLOR_MATRIX_COLOR_TABLE 0x80D5
#endif
#ifndef GL_PROXY_POST_COLOR_MATRIX_COLOR_TABLE_SGI
#define GL_PROXY_POST_COLOR_MATRIX_COLOR_TABLE_SGI 0x80D5
#endif
#ifndef GL_COLOR_TABLE_SCALE
#define GL_COLOR_TABLE_SCALE 0x80D6
#endif
#ifndef GL_COLOR_TABLE_SCALE_SGI
#define GL_COLOR_TABLE_SCALE_SGI 0x80D6
#endif
#ifndef GL_COLOR_TABLE_BIAS
#define GL_COLOR_TABLE_BIAS 0x80D7
#endif
#ifndef GL_COLOR_TABLE_BIAS_SGI
#define GL_COLOR_TABLE_BIAS_SGI 0x80D7
#endif
#ifndef GL_COLOR_TABLE_FORMAT
#define GL_COLOR_TABLE_FORMAT 0x80D8
#endif
#ifndef GL_COLOR_TABLE_FORMAT_SGI
#define GL_COLOR_TABLE_FORMAT_SGI 0x80D8
#endif
#ifndef GL_COLOR_TABLE_WIDTH
#define GL_COLOR_TABLE_WIDTH 0x80D9
#endif
#ifndef GL_COLOR_TABLE_WIDTH_SGI
#define GL_COLOR_TABLE_WIDTH_SGI 0x80D9
#endif
#ifndef GL_COLOR_TABLE_RED_SIZE
#define GL_COLOR_TABLE_RED_SIZE 0x80DA
#endif
#ifndef GL_COLOR_TABLE_RED_SIZE_SGI
#define GL_COLOR_TABLE_RED_SIZE_SGI 0x80DA
#endif
#ifndef GL_COLOR_TABLE_GREEN_SIZE
#define GL_COLOR_TABLE_GREEN_SIZE 0x80DB
#endif
#ifndef GL_COLOR_TABLE_GREEN_SIZE_SGI
#define GL_COLOR_TABLE_GREEN_SIZE_SGI 0x80DB
#endif
#ifndef GL_COLOR_TABLE_BLUE_SIZE
#define GL_COLOR_TABLE_BLUE_SIZE 0x80DC
#endif
#ifndef GL_COLOR_TABLE_BLUE_SIZE_SGI
#define GL_COLOR_TABLE_BLUE_SIZE_SGI 0x80DC
#endif
#ifndef GL_COLOR_TABLE_ALPHA_SIZE
#define GL_COLOR_TABLE_ALPHA_SIZE 0x80DD
#endif
#ifndef GL_COLOR_TABLE_ALPHA_SIZE_SGI
#define GL_COLOR_TABLE_ALPHA_SIZE_SGI 0x80DD
#endif
#ifndef GL_COLOR_TABLE_LUMINANCE_SIZE
#define GL_COLOR_TABLE_LUMINANCE_SIZE 0x80DE
#endif
#ifndef GL_COLOR_TABLE_LUMINANCE_SIZE_SGI
#define GL_COLOR_TABLE_LUMINANCE_SIZE_SGI 0x80DE
#endif
#ifndef GL_COLOR_TABLE_INTENSITY_SIZE
#define GL_COLOR_TABLE_INTENSITY_SIZE 0x80DF
#endif
#ifndef GL_COLOR_TABLE_INTENSITY_SIZE_SGI
#define GL_COLOR_TABLE_INTENSITY_SIZE_SGI 0x80DF
#endif
#ifndef GL_BGR
#define GL_BGR 0x80E0
#endif
#ifndef GL_BGR_EXT
#define GL_BGR_EXT 0x80E0
#endif
#ifndef GL_BGRA
#define GL_BGRA 0x80E1
#endif
#ifndef GL_BGRA_EXT
#define GL_BGRA_EXT 0x80E1
#endif
#ifndef GL_MAX_ELEMENTS_VERTICES
#define GL_MAX_ELEMENTS_VERTICES 0x80E8
#endif
#ifndef GL_MAX_ELEMENTS_INDICES
#define GL_MAX_ELEMENTS_INDICES 0x80E9
#endif
#ifndef GL_DUAL_ALPHA4_SGIS
#define GL_DUAL_ALPHA4_SGIS 0x8110
#endif
#ifndef GL_DUAL_ALPHA8_SGIS
#define GL_DUAL_ALPHA8_SGIS 0x8111
#endif
#ifndef GL_DUAL_ALPHA12_SGIS
#define GL_DUAL_ALPHA12_SGIS 0x8112
#endif
#ifndef GL_DUAL_ALPHA16_SGIS
#define GL_DUAL_ALPHA16_SGIS 0x8113
#endif
#ifndef GL_DUAL_LUMINANCE4_SGIS
#define GL_DUAL_LUMINANCE4_SGIS 0x8114
#endif
#ifndef GL_DUAL_LUMINANCE8_SGIS
#define GL_DUAL_LUMINANCE8_SGIS 0x8115
#endif
#ifndef GL_DUAL_LUMINANCE12_SGIS
#define GL_DUAL_LUMINANCE12_SGIS 0x8116
#endif
#ifndef GL_DUAL_LUMINANCE16_SGIS
#define GL_DUAL_LUMINANCE16_SGIS 0x8117
#endif
#ifndef GL_DUAL_INTENSITY4_SGIS
#define GL_DUAL_INTENSITY4_SGIS 0x8118
#endif
#ifndef GL_DUAL_INTENSITY8_SGIS
#define GL_DUAL_INTENSITY8_SGIS 0x8119
#endif
#ifndef GL_DUAL_INTENSITY12_SGIS
#define GL_DUAL_INTENSITY12_SGIS 0x811A
#endif
#ifndef GL_DUAL_INTENSITY16_SGIS
#define GL_DUAL_INTENSITY16_SGIS 0x811B
#endif
#ifndef GL_DUAL_LUMINANCE_ALPHA4_SGIS
#define GL_DUAL_LUMINANCE_ALPHA4_SGIS 0x811C
#endif
#ifndef GL_DUAL_LUMINANCE_ALPHA8_SGIS
#define GL_DUAL_LUMINANCE_ALPHA8_SGIS 0x811D
#endif
#ifndef GL_QUAD_ALPHA4_SGIS
#define GL_QUAD_ALPHA4_SGIS 0x811E
#endif
#ifndef GL_QUAD_ALPHA8_SGIS
#define GL_QUAD_ALPHA8_SGIS 0x811F
#endif
#ifndef GL_QUAD_LUMINANCE4_SGIS
#define GL_QUAD_LUMINANCE4_SGIS 0x8120
#endif
#ifndef GL_QUAD_LUMINANCE8_SGIS
#define GL_QUAD_LUMINANCE8_SGIS 0x8121
#endif
#ifndef GL_QUAD_INTENSITY4_SGIS
#define GL_QUAD_INTENSITY4_SGIS 0x8122
#endif
#ifndef GL_QUAD_INTENSITY8_SGIS
#define GL_QUAD_INTENSITY8_SGIS 0x8123
#endif
#ifndef GL_DUAL_TEXTURE_SELECT_SGIS
#define GL_DUAL_TEXTURE_SELECT_SGIS 0x8124
#endif
#ifndef GL_QUAD_TEXTURE_SELECT_SGIS
#define GL_QUAD_TEXTURE_SELECT_SGIS 0x8125
#endif
#ifndef GL_POINT_SIZE_MIN
#define GL_POINT_SIZE_MIN 0x8126
#endif
#ifndef GL_POINT_SIZE_MIN_ARB
#define GL_POINT_SIZE_MIN_ARB 0x8126
#endif
#ifndef GL_POINT_SIZE_MIN_EXT
#define GL_POINT_SIZE_MIN_EXT 0x8126
#endif
#ifndef GL_POINT_SIZE_MIN_SGIS
#define GL_POINT_SIZE_MIN_SGIS 0x8126
#endif
#ifndef GL_POINT_SIZE_MAX
#define GL_POINT_SIZE_MAX 0x8127
#endif
#ifndef GL_POINT_SIZE_MAX_ARB
#define GL_POINT_SIZE_MAX_ARB 0x8127
#endif
#ifndef GL_POINT_SIZE_MAX_EXT
#define GL_POINT_SIZE_MAX_EXT 0x8127
#endif
#ifndef GL_POINT_SIZE_MAX_SGIS
#define GL_POINT_SIZE_MAX_SGIS 0x8127
#endif
#ifndef GL_POINT_FADE_THRESHOLD_SIZE
#define GL_POINT_FADE_THRESHOLD_SIZE 0x8128
#endif
#ifndef GL_POINT_FADE_THRESHOLD_SIZE_ARB
#define GL_POINT_FADE_THRESHOLD_SIZE_ARB 0x8128
#endif
#ifndef GL_POINT_FADE_THRESHOLD_SIZE_EXT
#define GL_POINT_FADE_THRESHOLD_SIZE_EXT 0x8128
#endif
#ifndef GL_POINT_FADE_THRESHOLD_SIZE_SGIS
#define GL_POINT_FADE_THRESHOLD_SIZE_SGIS 0x8128
#endif
#ifndef GL_POINT_DISTANCE_ATTENUATION
#define GL_POINT_DISTANCE_ATTENUATION 0x8129
#endif
#ifndef GL_POINT_DISTANCE_ATTENUATION_ARB
#define GL_POINT_DISTANCE_ATTENUATION_ARB 0x8129
#endif
#ifndef GL_DISTANCE_ATTENUATION_EXT
#define GL_DISTANCE_ATTENUATION_EXT 0x8129
#endif
#ifndef GL_DISTANCE_ATTENUATION_SGIS
#define GL_DISTANCE_ATTENUATION_SGIS 0x8129
#endif
#ifndef GL_FOG_FUNC_SGIS
#define GL_FOG_FUNC_SGIS 0x812A
#endif
#ifndef GL_FOG_FUNC_POINTS_SGIS
#define GL_FOG_FUNC_POINTS_SGIS 0x812B
#endif
#ifndef GL_MAX_FOG_FUNC_POINTS_SGIS
#define GL_MAX_FOG_FUNC_POINTS_SGIS 0x812C
#endif
#ifndef GL_CLAMP_TO_BORDER
#define GL_CLAMP_TO_BORDER 0x812D
#endif
#ifndef GL_CLAMP_TO_BORDER_ARB
#define GL_CLAMP_TO_BORDER_ARB 0x812D
#endif
#ifndef GL_CLAMP_TO_BORDER_SGIS
#define GL_CLAMP_TO_BORDER_SGIS 0x812D
#endif
#ifndef GL_TEXTURE_MULTI_BUFFER_HINT_SGIX
#define GL_TEXTURE_MULTI_BUFFER_HINT_SGIX 0x812E
#endif
#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif
#ifndef GL_CLAMP_TO_EDGE_SGIS
#define GL_CLAMP_TO_EDGE_SGIS 0x812F
#endif
#ifndef GL_PACK_SKIP_VOLUMES_SGIS
#define GL_PACK_SKIP_VOLUMES_SGIS 0x8130
#endif
#ifndef GL_PACK_IMAGE_DEPTH_SGIS
#define GL_PACK_IMAGE_DEPTH_SGIS 0x8131
#endif
#ifndef GL_UNPACK_SKIP_VOLUMES_SGIS
#define GL_UNPACK_SKIP_VOLUMES_SGIS 0x8132
#endif
#ifndef GL_UNPACK_IMAGE_DEPTH_SGIS
#define GL_UNPACK_IMAGE_DEPTH_SGIS 0x8133
#endif
#ifndef GL_TEXTURE_4D_SGIS
#define GL_TEXTURE_4D_SGIS 0x8134
#endif
#ifndef GL_PROXY_TEXTURE_4D_SGIS
#define GL_PROXY_TEXTURE_4D_SGIS 0x8135
#endif
#ifndef GL_TEXTURE_4DSIZE_SGIS
#define GL_TEXTURE_4DSIZE_SGIS 0x8136
#endif
#ifndef GL_TEXTURE_WRAP_Q_SGIS
#define GL_TEXTURE_WRAP_Q_SGIS 0x8137
#endif
#ifndef GL_MAX_4D_TEXTURE_SIZE_SGIS
#define GL_MAX_4D_TEXTURE_SIZE_SGIS 0x8138
#endif
#ifndef GL_TEXTURE_4D_BINDING_SGIS
#define GL_TEXTURE_4D_BINDING_SGIS 0x814F
#endif
#ifndef GL_PIXEL_TEX_GEN_SGIX
#define GL_PIXEL_TEX_GEN_SGIX 0x8139
#endif
#ifndef GL_PIXEL_TEX_GEN_MODE_SGIX
#define GL_PIXEL_TEX_GEN_MODE_SGIX 0x832B
#endif
#ifndef GL_TEXTURE_MIN_LOD
#define GL_TEXTURE_MIN_LOD 0x813A
#endif
#ifndef GL_TEXTURE_MIN_LOD_SGIS
#define GL_TEXTURE_MIN_LOD_SGIS 0x813A
#endif
#ifndef GL_TEXTURE_MAX_LOD
#define GL_TEXTURE_MAX_LOD 0x813B
#endif
#ifndef GL_TEXTURE_MAX_LOD_SGIS
#define GL_TEXTURE_MAX_LOD_SGIS 0x813B
#endif
#ifndef GL_TEXTURE_BASE_LEVEL
#define GL_TEXTURE_BASE_LEVEL 0x813C
#endif
#ifndef GL_TEXTURE_BASE_LEVEL_SGIS
#define GL_TEXTURE_BASE_LEVEL_SGIS 0x813C
#endif
#ifndef GL_TEXTURE_MAX_LEVEL
#define GL_TEXTURE_MAX_LEVEL 0x813D
#endif
#ifndef GL_TEXTURE_MAX_LEVEL_SGIS
#define GL_TEXTURE_MAX_LEVEL_SGIS 0x813D
#endif
#ifndef GL_PIXEL_TILE_BEST_ALIGNMENT_SGIX
#define GL_PIXEL_TILE_BEST_ALIGNMENT_SGIX 0x813E
#endif
#ifndef GL_PIXEL_TILE_CACHE_INCREMENT_SGIX
#define GL_PIXEL_TILE_CACHE_INCREMENT_SGIX 0x813F
#endif
#ifndef GL_PIXEL_TILE_WIDTH_SGIX
#define GL_PIXEL_TILE_WIDTH_SGIX 0x8140
#endif
#ifndef GL_PIXEL_TILE_HEIGHT_SGIX
#define GL_PIXEL_TILE_HEIGHT_SGIX 0x8141
#endif
#ifndef GL_PIXEL_TILE_GRID_WIDTH_SGIX
#define GL_PIXEL_TILE_GRID_WIDTH_SGIX 0x8142
#endif
#ifndef GL_PIXEL_TILE_GRID_HEIGHT_SGIX
#define GL_PIXEL_TILE_GRID_HEIGHT_SGIX 0x8143
#endif
#ifndef GL_PIXEL_TILE_GRID_DEPTH_SGIX
#define GL_PIXEL_TILE_GRID_DEPTH_SGIX 0x8144
#endif
#ifndef GL_PIXEL_TILE_CACHE_SIZE_SGIX
#define GL_PIXEL_TILE_CACHE_SIZE_SGIX 0x8145
#endif
#ifndef GL_FILTER4_SGIS
#define GL_FILTER4_SGIS 0x8146
#endif
#ifndef GL_TEXTURE_FILTER4_SIZE_SGIS
#define GL_TEXTURE_FILTER4_SIZE_SGIS 0x8147
#endif
#ifndef GL_SPRITE_SGIX
#define GL_SPRITE_SGIX 0x8148
#endif
#ifndef GL_SPRITE_MODE_SGIX
#define GL_SPRITE_MODE_SGIX 0x8149
#endif
#ifndef GL_SPRITE_AXIS_SGIX
#define GL_SPRITE_AXIS_SGIX 0x814A
#endif
#ifndef GL_SPRITE_TRANSLATION_SGIX
#define GL_SPRITE_TRANSLATION_SGIX 0x814B
#endif
#ifndef GL_SPRITE_AXIAL_SGIX
#define GL_SPRITE_AXIAL_SGIX 0x814C
#endif
#ifndef GL_SPRITE_OBJECT_ALIGNED_SGIX
#define GL_SPRITE_OBJECT_ALIGNED_SGIX 0x814D
#endif
#ifndef GL_SPRITE_EYE_ALIGNED_SGIX
#define GL_SPRITE_EYE_ALIGNED_SGIX 0x814E
#endif
#ifndef GL_IGNORE_BORDER_HP
#define GL_IGNORE_BORDER_HP 0x8150
#endif
#ifndef GL_CONSTANT_BORDER
#define GL_CONSTANT_BORDER 0x8151
#endif
#ifndef GL_CONSTANT_BORDER_HP
#define GL_CONSTANT_BORDER_HP 0x8151
#endif
#ifndef GL_REPLICATE_BORDER
#define GL_REPLICATE_BORDER 0x8153
#endif
#ifndef GL_REPLICATE_BORDER_HP
#define GL_REPLICATE_BORDER_HP 0x8153
#endif
#ifndef GL_CONVOLUTION_BORDER_COLOR
#define GL_CONVOLUTION_BORDER_COLOR 0x8154
#endif
#ifndef GL_CONVOLUTION_BORDER_COLOR_HP
#define GL_CONVOLUTION_BORDER_COLOR_HP 0x8154
#endif
#ifndef GL_LINEAR_CLIPMAP_LINEAR_SGIX
#define GL_LINEAR_CLIPMAP_LINEAR_SGIX 0x8170
#endif
#ifndef GL_TEXTURE_CLIPMAP_CENTER_SGIX
#define GL_TEXTURE_CLIPMAP_CENTER_SGIX 0x8171
#endif
#ifndef GL_TEXTURE_CLIPMAP_FRAME_SGIX
#define GL_TEXTURE_CLIPMAP_FRAME_SGIX 0x8172
#endif
#ifndef GL_TEXTURE_CLIPMAP_OFFSET_SGIX
#define GL_TEXTURE_CLIPMAP_OFFSET_SGIX 0x8173
#endif
#ifndef GL_TEXTURE_CLIPMAP_VIRTUAL_DEPTH_SGIX
#define GL_TEXTURE_CLIPMAP_VIRTUAL_DEPTH_SGIX 0x8174
#endif
#ifndef GL_TEXTURE_CLIPMAP_LOD_OFFSET_SGIX
#define GL_TEXTURE_CLIPMAP_LOD_OFFSET_SGIX 0x8175
#endif
#ifndef GL_TEXTURE_CLIPMAP_DEPTH_SGIX
#define GL_TEXTURE_CLIPMAP_DEPTH_SGIX 0x8176
#endif
#ifndef GL_MAX_CLIPMAP_DEPTH_SGIX
#define GL_MAX_CLIPMAP_DEPTH_SGIX 0x8177
#endif
#ifndef GL_MAX_CLIPMAP_VIRTUAL_DEPTH_SGIX
#define GL_MAX_CLIPMAP_VIRTUAL_DEPTH_SGIX 0x8178
#endif
#ifndef GL_NEAREST_CLIPMAP_NEAREST_SGIX
#define GL_NEAREST_CLIPMAP_NEAREST_SGIX 0x844D
#endif
#ifndef GL_NEAREST_CLIPMAP_LINEAR_SGIX
#define GL_NEAREST_CLIPMAP_LINEAR_SGIX 0x844E
#endif
#ifndef GL_LINEAR_CLIPMAP_NEAREST_SGIX
#define GL_LINEAR_CLIPMAP_NEAREST_SGIX 0x844F
#endif
#ifndef GL_POST_TEXTURE_FILTER_BIAS_SGIX
#define GL_POST_TEXTURE_FILTER_BIAS_SGIX 0x8179
#endif
#ifndef GL_POST_TEXTURE_FILTER_SCALE_SGIX
#define GL_POST_TEXTURE_FILTER_SCALE_SGIX 0x817A
#endif
#ifndef GL_POST_TEXTURE_FILTER_BIAS_RANGE_SGIX
#define GL_POST_TEXTURE_FILTER_BIAS_RANGE_SGIX 0x817B
#endif
#ifndef GL_POST_TEXTURE_FILTER_SCALE_RANGE_SGIX
#define GL_POST_TEXTURE_FILTER_SCALE_RANGE_SGIX 0x817C
#endif
#ifndef GL_REFERENCE_PLANE_SGIX
#define GL_REFERENCE_PLANE_SGIX 0x817D
#endif
#ifndef GL_REFERENCE_PLANE_EQUATION_SGIX
#define GL_REFERENCE_PLANE_EQUATION_SGIX 0x817E
#endif
#ifndef GL_IR_INSTRUMENT1_SGIX
#define GL_IR_INSTRUMENT1_SGIX 0x817F
#endif
#ifndef GL_INSTRUMENT_BUFFER_POINTER_SGIX
#define GL_INSTRUMENT_BUFFER_POINTER_SGIX 0x8180
#endif
#ifndef GL_INSTRUMENT_MEASUREMENTS_SGIX
#define GL_INSTRUMENT_MEASUREMENTS_SGIX 0x8181
#endif
#ifndef GL_LIST_PRIORITY_SGIX
#define GL_LIST_PRIORITY_SGIX 0x8182
#endif
#ifndef GL_CALLIGRAPHIC_FRAGMENT_SGIX
#define GL_CALLIGRAPHIC_FRAGMENT_SGIX 0x8183
#endif
#ifndef GL_PIXEL_TEX_GEN_Q_CEILING_SGIX
#define GL_PIXEL_TEX_GEN_Q_CEILING_SGIX 0x8184
#endif
#ifndef GL_PIXEL_TEX_GEN_Q_ROUND_SGIX
#define GL_PIXEL_TEX_GEN_Q_ROUND_SGIX 0x8185
#endif
#ifndef GL_PIXEL_TEX_GEN_Q_FLOOR_SGIX
#define GL_PIXEL_TEX_GEN_Q_FLOOR_SGIX 0x8186
#endif
#ifndef GL_PIXEL_TEX_GEN_ALPHA_REPLACE_SGIX
#define GL_PIXEL_TEX_GEN_ALPHA_REPLACE_SGIX 0x8187
#endif
#ifndef GL_PIXEL_TEX_GEN_ALPHA_NO_REPLACE_SGIX
#define GL_PIXEL_TEX_GEN_ALPHA_NO_REPLACE_SGIX 0x8188
#endif
#ifndef GL_PIXEL_TEX_GEN_ALPHA_LS_SGIX
#define GL_PIXEL_TEX_GEN_ALPHA_LS_SGIX 0x8189
#endif
#ifndef GL_PIXEL_TEX_GEN_ALPHA_MS_SGIX
#define GL_PIXEL_TEX_GEN_ALPHA_MS_SGIX 0x818A
#endif
#ifndef GL_FRAMEZOOM_SGIX
#define GL_FRAMEZOOM_SGIX 0x818B
#endif
#ifndef GL_FRAMEZOOM_FACTOR_SGIX
#define GL_FRAMEZOOM_FACTOR_SGIX 0x818C
#endif
#ifndef GL_MAX_FRAMEZOOM_FACTOR_SGIX
#define GL_MAX_FRAMEZOOM_FACTOR_SGIX 0x818D
#endif
#ifndef GL_TEXTURE_LOD_BIAS_S_SGIX
#define GL_TEXTURE_LOD_BIAS_S_SGIX 0x818E
#endif
#ifndef GL_TEXTURE_LOD_BIAS_T_SGIX
#define GL_TEXTURE_LOD_BIAS_T_SGIX 0x818F
#endif
#ifndef GL_TEXTURE_LOD_BIAS_R_SGIX
#define GL_TEXTURE_LOD_BIAS_R_SGIX 0x8190
#endif
#ifndef GL_GENERATE_MIPMAP
#define GL_GENERATE_MIPMAP 0x8191
#endif
#ifndef GL_GENERATE_MIPMAP_SGIS
#define GL_GENERATE_MIPMAP_SGIS 0x8191
#endif
#ifndef GL_GENERATE_MIPMAP_HINT
#define GL_GENERATE_MIPMAP_HINT 0x8192
#endif
#ifndef GL_GENERATE_MIPMAP_HINT_SGIS
#define GL_GENERATE_MIPMAP_HINT_SGIS 0x8192
#endif
#ifndef GL_GEOMETRY_DEFORMATION_SGIX
#define GL_GEOMETRY_DEFORMATION_SGIX 0x8194
#endif
#ifndef GL_TEXTURE_DEFORMATION_SGIX
#define GL_TEXTURE_DEFORMATION_SGIX 0x8195
#endif
#ifndef GL_DEFORMATIONS_MASK_SGIX
#define GL_DEFORMATIONS_MASK_SGIX 0x8196
#endif
#ifndef GL_MAX_DEFORMATION_ORDER_SGIX
#define GL_MAX_DEFORMATION_ORDER_SGIX 0x8197
#endif
#ifndef GL_FOG_OFFSET_SGIX
#define GL_FOG_OFFSET_SGIX 0x8198
#endif
#ifndef GL_FOG_OFFSET_VALUE_SGIX
#define GL_FOG_OFFSET_VALUE_SGIX 0x8199
#endif
#ifndef GL_TEXTURE_COMPARE_SGIX
#define GL_TEXTURE_COMPARE_SGIX 0x819A
#endif
#ifndef GL_TEXTURE_COMPARE_OPERATOR_SGIX
#define GL_TEXTURE_COMPARE_OPERATOR_SGIX 0x819B
#endif
#ifndef GL_TEXTURE_LEQUAL_R_SGIX
#define GL_TEXTURE_LEQUAL_R_SGIX 0x819C
#endif
#ifndef GL_TEXTURE_GEQUAL_R_SGIX
#define GL_TEXTURE_GEQUAL_R_SGIX 0x819D
#endif
#ifndef GL_DEPTH_COMPONENT16
#define GL_DEPTH_COMPONENT16 0x81A5
#endif
#ifndef GL_DEPTH_COMPONENT16_SGIX
#define GL_DEPTH_COMPONENT16_SGIX 0x81A5
#endif
#ifndef GL_DEPTH_COMPONENT24
#define GL_DEPTH_COMPONENT24 0x81A6
#endif
#ifndef GL_DEPTH_COMPONENT24_SGIX
#define GL_DEPTH_COMPONENT24_SGIX 0x81A6
#endif
#ifndef GL_DEPTH_COMPONENT32
#define GL_DEPTH_COMPONENT32 0x81A7
#endif
#ifndef GL_DEPTH_COMPONENT32_SGIX
#define GL_DEPTH_COMPONENT32_SGIX 0x81A7
#endif
#ifndef GL_YCRCB_422_SGIX
#define GL_YCRCB_422_SGIX 0x81BB
#endif
#ifndef GL_YCRCB_444_SGIX
#define GL_YCRCB_444_SGIX 0x81BC
#endif
#ifndef GL_TEXTURE_COLOR_WRITEMASK_SGIS
#define GL_TEXTURE_COLOR_WRITEMASK_SGIS 0x81EF
#endif
#ifndef GL_EYE_DISTANCE_TO_POINT_SGIS
#define GL_EYE_DISTANCE_TO_POINT_SGIS 0x81F0
#endif
#ifndef GL_OBJECT_DISTANCE_TO_POINT_SGIS
#define GL_OBJECT_DISTANCE_TO_POINT_SGIS 0x81F1
#endif
#ifndef GL_EYE_DISTANCE_TO_LINE_SGIS
#define GL_EYE_DISTANCE_TO_LINE_SGIS 0x81F2
#endif
#ifndef GL_OBJECT_DISTANCE_TO_LINE_SGIS
#define GL_OBJECT_DISTANCE_TO_LINE_SGIS 0x81F3
#endif
#ifndef GL_EYE_POINT_SGIS
#define GL_EYE_POINT_SGIS 0x81F4
#endif
#ifndef GL_OBJECT_POINT_SGIS
#define GL_OBJECT_POINT_SGIS 0x81F5
#endif
#ifndef GL_EYE_LINE_SGIS
#define GL_EYE_LINE_SGIS 0x81F6
#endif
#ifndef GL_OBJECT_LINE_SGIS
#define GL_OBJECT_LINE_SGIS 0x81F7
#endif
#ifndef GL_LIGHT_MODEL_COLOR_CONTROL
#define GL_LIGHT_MODEL_COLOR_CONTROL 0x81F8
#endif
#ifndef GL_LIGHT_MODEL_COLOR_CONTROL_EXT
#define GL_LIGHT_MODEL_COLOR_CONTROL_EXT 0x81F8
#endif
#ifndef GL_SINGLE_COLOR
#define GL_SINGLE_COLOR 0x81F9
#endif
#ifndef GL_SINGLE_COLOR_EXT
#define GL_SINGLE_COLOR_EXT 0x81F9
#endif
#ifndef GL_SEPARATE_SPECULAR_COLOR
#define GL_SEPARATE_SPECULAR_COLOR 0x81FA
#endif
#ifndef GL_SEPARATE_SPECULAR_COLOR_EXT
#define GL_SEPARATE_SPECULAR_COLOR_EXT 0x81FA
#endif
#ifndef GL_SHARED_TEXTURE_PALETTE_EXT
#define GL_SHARED_TEXTURE_PALETTE_EXT 0x81FB
#endif
#ifndef GL_CONVOLUTION_HINT_SGIX
#define GL_CONVOLUTION_HINT_SGIX 0x8316
#endif
#ifndef GL_ALPHA_MIN_SGIX
#define GL_ALPHA_MIN_SGIX 0x8320
#endif
#ifndef GL_ALPHA_MAX_SGIX
#define GL_ALPHA_MAX_SGIX 0x8321
#endif
#ifndef GL_ASYNC_MARKER_SGIX
#define GL_ASYNC_MARKER_SGIX 0x8329
#endif
#ifndef GL_ASYNC_HISTOGRAM_SGIX
#define GL_ASYNC_HISTOGRAM_SGIX 0x832C
#endif
#ifndef GL_MAX_ASYNC_HISTOGRAM_SGIX
#define GL_MAX_ASYNC_HISTOGRAM_SGIX 0x832D
#endif
#ifndef GL_PIXEL_TRANSFORM_2D_EXT
#define GL_PIXEL_TRANSFORM_2D_EXT 0x8330
#endif
#ifndef GL_PIXEL_MAG_FILTER_EXT
#define GL_PIXEL_MAG_FILTER_EXT 0x8331
#endif
#ifndef GL_PIXEL_MIN_FILTER_EXT
#define GL_PIXEL_MIN_FILTER_EXT 0x8332
#endif
#ifndef GL_PIXEL_CUBIC_WEIGHT_EXT
#define GL_PIXEL_CUBIC_WEIGHT_EXT 0x8333
#endif
#ifndef GL_CUBIC_EXT
#define GL_CUBIC_EXT 0x8334
#endif
#ifndef GL_AVERAGE_EXT
#define GL_AVERAGE_EXT 0x8335
#endif
#ifndef GL_PIXEL_TRANSFORM_2D_STACK_DEPTH_EXT
#define GL_PIXEL_TRANSFORM_2D_STACK_DEPTH_EXT 0x8336
#endif
#ifndef GL_MAX_PIXEL_TRANSFORM_2D_STACK_DEPTH_EXT
#define GL_MAX_PIXEL_TRANSFORM_2D_STACK_DEPTH_EXT 0x8337
#endif
#ifndef GL_PIXEL_TRANSFORM_2D_MATRIX_EXT
#define GL_PIXEL_TRANSFORM_2D_MATRIX_EXT 0x8338
#endif
#ifndef GL_PIXEL_TEXTURE_SGIS
#define GL_PIXEL_TEXTURE_SGIS 0x8353
#endif
#ifndef GL_PIXEL_FRAGMENT_RGB_SOURCE_SGIS
#define GL_PIXEL_FRAGMENT_RGB_SOURCE_SGIS 0x8354
#endif
#ifndef GL_PIXEL_FRAGMENT_ALPHA_SOURCE_SGIS
#define GL_PIXEL_FRAGMENT_ALPHA_SOURCE_SGIS 0x8355
#endif
#ifndef GL_PIXEL_GROUP_COLOR_SGIS
#define GL_PIXEL_GROUP_COLOR_SGIS 0x8356
#endif
#ifndef GL_ASYNC_TEX_IMAGE_SGIX
#define GL_ASYNC_TEX_IMAGE_SGIX 0x835C
#endif
#ifndef GL_ASYNC_DRAW_PIXELS_SGIX
#define GL_ASYNC_DRAW_PIXELS_SGIX 0x835D
#endif
#ifndef GL_ASYNC_READ_PIXELS_SGIX
#define GL_ASYNC_READ_PIXELS_SGIX 0x835E
#endif
#ifndef GL_MAX_ASYNC_TEX_IMAGE_SGIX
#define GL_MAX_ASYNC_TEX_IMAGE_SGIX 0x835F
#endif
#ifndef GL_MAX_ASYNC_DRAW_PIXELS_SGIX
#define GL_MAX_ASYNC_DRAW_PIXELS_SGIX 0x8360
#endif
#ifndef GL_MAX_ASYNC_READ_PIXELS_SGIX
#define GL_MAX_ASYNC_READ_PIXELS_SGIX 0x8361
#endif
#ifndef GL_TEXTURE_MAX_CLAMP_S_SGIX
#define GL_TEXTURE_MAX_CLAMP_S_SGIX 0x8369
#endif
#ifndef GL_TEXTURE_MAX_CLAMP_T_SGIX
#define GL_TEXTURE_MAX_CLAMP_T_SGIX 0x836A
#endif
#ifndef GL_TEXTURE_MAX_CLAMP_R_SGIX
#define GL_TEXTURE_MAX_CLAMP_R_SGIX 0x836B
#endif
#ifndef GL_FOG_FACTOR_TO_ALPHA_SGIX
#define GL_FOG_FACTOR_TO_ALPHA_SGIX 0x836F
#endif
#ifndef GL_VERTEX_PRECLIP_SGIX
#define GL_VERTEX_PRECLIP_SGIX 0x83EE
#endif
#ifndef GL_VERTEX_PRECLIP_HINT_SGIX
#define GL_VERTEX_PRECLIP_HINT_SGIX 0x83EF
#endif
#ifndef GL_COMPRESSED_RGB_S3TC_DXT1_EXT
#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT 0x83F0
#endif
#ifndef GL_COMPRESSED_RGBA_S3TC_DXT1_EXT
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT 0x83F1
#endif
#ifndef GL_COMPRESSED_RGBA_S3TC_DXT3_EXT
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT 0x83F2
#endif
#ifndef GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT 0x83F3
#endif
#ifndef GL_PARALLEL_ARRAYS_INTEL
#define GL_PARALLEL_ARRAYS_INTEL 0x83F4
#endif
#ifndef GL_VERTEX_ARRAY_PARALLEL_POINTERS_INTEL
#define GL_VERTEX_ARRAY_PARALLEL_POINTERS_INTEL 0x83F5
#endif
#ifndef GL_NORMAL_ARRAY_PARALLEL_POINTERS_INTEL
#define GL_NORMAL_ARRAY_PARALLEL_POINTERS_INTEL 0x83F6
#endif
#ifndef GL_COLOR_ARRAY_PARALLEL_POINTERS_INTEL
#define GL_COLOR_ARRAY_PARALLEL_POINTERS_INTEL 0x83F7
#endif
#ifndef GL_TEXTURE_COORD_ARRAY_PARALLEL_POINTERS_INTEL
#define GL_TEXTURE_COORD_ARRAY_PARALLEL_POINTERS_INTEL 0x83F8
#endif
#ifndef GL_FRAGMENT_LIGHTING_SGIX
#define GL_FRAGMENT_LIGHTING_SGIX 0x8400
#endif
#ifndef GL_FRAGMENT_COLOR_MATERIAL_SGIX
#define GL_FRAGMENT_COLOR_MATERIAL_SGIX 0x8401
#endif
#ifndef GL_FRAGMENT_COLOR_MATERIAL_FACE_SGIX
#define GL_FRAGMENT_COLOR_MATERIAL_FACE_SGIX 0x8402
#endif
#ifndef GL_FRAGMENT_COLOR_MATERIAL_PARAMETER_SGIX
#define GL_FRAGMENT_COLOR_MATERIAL_PARAMETER_SGIX 0x8403
#endif
#ifndef GL_MAX_FRAGMENT_LIGHTS_SGIX
#define GL_MAX_FRAGMENT_LIGHTS_SGIX 0x8404
#endif
#ifndef GL_MAX_ACTIVE_LIGHTS_SGIX
#define GL_MAX_ACTIVE_LIGHTS_SGIX 0x8405
#endif
#ifndef GL_CURRENT_RASTER_NORMAL_SGIX
#define GL_CURRENT_RASTER_NORMAL_SGIX 0x8406
#endif
#ifndef GL_LIGHT_ENV_MODE_SGIX
#define GL_LIGHT_ENV_MODE_SGIX 0x8407
#endif
#ifndef GL_FRAGMENT_LIGHT_MODEL_LOCAL_VIEWER_SGIX
#define GL_FRAGMENT_LIGHT_MODEL_LOCAL_VIEWER_SGIX 0x8408
#endif
#ifndef GL_FRAGMENT_LIGHT_MODEL_TWO_SIDE_SGIX
#define GL_FRAGMENT_LIGHT_MODEL_TWO_SIDE_SGIX 0x8409
#endif
#ifndef GL_FRAGMENT_LIGHT_MODEL_AMBIENT_SGIX
#define GL_FRAGMENT_LIGHT_MODEL_AMBIENT_SGIX 0x840A
#endif
#ifndef GL_FRAGMENT_LIGHT_MODEL_NORMAL_INTERPOLATION_SGIX
#define GL_FRAGMENT_LIGHT_MODEL_NORMAL_INTERPOLATION_SGIX 0x840B
#endif
#ifndef GL_FRAGMENT_LIGHT0_SGIX
#define GL_FRAGMENT_LIGHT0_SGIX 0x840C
#endif
#ifndef GL_FRAGMENT_LIGHT1_SGIX
#define GL_FRAGMENT_LIGHT1_SGIX 0x840D
#endif
#ifndef GL_FRAGMENT_LIGHT2_SGIX
#define GL_FRAGMENT_LIGHT2_SGIX 0x840E
#endif
#ifndef GL_FRAGMENT_LIGHT3_SGIX
#define GL_FRAGMENT_LIGHT3_SGIX 0x840F
#endif
#ifndef GL_FRAGMENT_LIGHT4_SGIX
#define GL_FRAGMENT_LIGHT4_SGIX 0x8410
#endif
#ifndef GL_FRAGMENT_LIGHT5_SGIX
#define GL_FRAGMENT_LIGHT5_SGIX 0x8411
#endif
#ifndef GL_FRAGMENT_LIGHT6_SGIX
#define GL_FRAGMENT_LIGHT6_SGIX 0x8412
#endif
#ifndef GL_FRAGMENT_LIGHT7_SGIX
#define GL_FRAGMENT_LIGHT7_SGIX 0x8413
#endif
#ifndef GL_PACK_RESAMPLE_SGIX
#define GL_PACK_RESAMPLE_SGIX 0x842C
#endif
#ifndef GL_UNPACK_RESAMPLE_SGIX
#define GL_UNPACK_RESAMPLE_SGIX 0x842D
#endif
#ifndef GL_RESAMPLE_REPLICATE_SGIX
#define GL_RESAMPLE_REPLICATE_SGIX 0x842E
#endif
#ifndef GL_RESAMPLE_ZERO_FILL_SGIX
#define GL_RESAMPLE_ZERO_FILL_SGIX 0x842F
#endif
#ifndef GL_RESAMPLE_DECIMATE_SGIX
#define GL_RESAMPLE_DECIMATE_SGIX 0x8430
#endif
#ifndef GL_SMOOTH_POINT_SIZE_RANGE
#define GL_SMOOTH_POINT_SIZE_RANGE 0x0B12
#endif
#ifndef GL_SMOOTH_POINT_SIZE_GRANULARITY
#define GL_SMOOTH_POINT_SIZE_GRANULARITY 0x0B13
#endif
#ifndef GL_SMOOTH_LINE_WIDTH_RANGE
#define GL_SMOOTH_LINE_WIDTH_RANGE 0x0B22
#endif
#ifndef GL_SMOOTH_LINE_WIDTH_GRANULARITY
#define GL_SMOOTH_LINE_WIDTH_GRANULARITY 0x0B23
#endif
#ifndef GL_ALIASED_POINT_SIZE_RANGE
#define GL_ALIASED_POINT_SIZE_RANGE 0x846D
#endif
#ifndef GL_ALIASED_LINE_WIDTH_RANGE
#define GL_ALIASED_LINE_WIDTH_RANGE 0x846E
#endif
#ifndef GL_PACK_SUBSAMPLE_RATE_SGIX
#define GL_PACK_SUBSAMPLE_RATE_SGIX 0x85A0
#endif
#ifndef GL_UNPACK_SUBSAMPLE_RATE_SGIX
#define GL_UNPACK_SUBSAMPLE_RATE_SGIX 0x85A1
#endif
#ifndef GL_PIXEL_SUBSAMPLE_4444_SGIX
#define GL_PIXEL_SUBSAMPLE_4444_SGIX 0x85A2
#endif
#ifndef GL_PIXEL_SUBSAMPLE_2424_SGIX
#define GL_PIXEL_SUBSAMPLE_2424_SGIX 0x85A3
#endif
#ifndef GL_PIXEL_SUBSAMPLE_4242_SGIX
#define GL_PIXEL_SUBSAMPLE_4242_SGIX 0x85A4
#endif
#ifndef GL_UNSIGNED_BYTE_3_3_2
#define GL_UNSIGNED_BYTE_3_3_2 0x8032
#endif
#ifndef GL_UNSIGNED_SHORT_4_4_4_4
#define GL_UNSIGNED_SHORT_4_4_4_4 0x8033
#endif
#ifndef GL_UNSIGNED_SHORT_5_5_5_1
#define GL_UNSIGNED_SHORT_5_5_5_1 0x8034
#endif
#ifndef GL_UNSIGNED_INT_8_8_8_8
#define GL_UNSIGNED_INT_8_8_8_8 0x8035
#endif
#ifndef GL_UNSIGNED_INT_10_10_10_2
#define GL_UNSIGNED_INT_10_10_10_2 0x8036
#endif
#ifndef GL_RESCALE_NORMAL
#define GL_RESCALE_NORMAL 0x803A
#endif
#ifndef GL_TEXTURE_BINDING_3D
#define GL_TEXTURE_BINDING_3D 0x806A
#endif
#ifndef GL_PACK_SKIP_IMAGES
#define GL_PACK_SKIP_IMAGES 0x806B
#endif
#ifndef GL_PACK_IMAGE_HEIGHT
#define GL_PACK_IMAGE_HEIGHT 0x806C
#endif
#ifndef GL_UNPACK_SKIP_IMAGES
#define GL_UNPACK_SKIP_IMAGES 0x806D
#endif
#ifndef GL_UNPACK_IMAGE_HEIGHT
#define GL_UNPACK_IMAGE_HEIGHT 0x806E
#endif
#ifndef GL_TEXTURE_3D
#define GL_TEXTURE_3D 0x806F
#endif
#ifndef GL_PROXY_TEXTURE_3D
#define GL_PROXY_TEXTURE_3D 0x8070
#endif
#ifndef GL_TEXTURE_DEPTH
#define GL_TEXTURE_DEPTH 0x8071
#endif
#ifndef GL_TEXTURE_WRAP_R
#define GL_TEXTURE_WRAP_R 0x8072
#endif
#ifndef GL_MAX_3D_TEXTURE_SIZE
#define GL_MAX_3D_TEXTURE_SIZE 0x8073
#endif
#ifndef GL_UNSIGNED_BYTE_2_3_3_REV
#define GL_UNSIGNED_BYTE_2_3_3_REV 0x8362
#endif
#ifndef GL_UNSIGNED_SHORT_5_6_5
#define GL_UNSIGNED_SHORT_5_6_5 0x8363
#endif
#ifndef GL_UNSIGNED_SHORT_5_6_5_REV
#define GL_UNSIGNED_SHORT_5_6_5_REV 0x8364
#endif
#ifndef GL_UNSIGNED_SHORT_4_4_4_4_REV
#define GL_UNSIGNED_SHORT_4_4_4_4_REV 0x8365
#endif
#ifndef GL_UNSIGNED_SHORT_1_5_5_5_REV
#define GL_UNSIGNED_SHORT_1_5_5_5_REV 0x8366
#endif
#ifndef GL_UNSIGNED_INT_8_8_8_8_REV
#define GL_UNSIGNED_INT_8_8_8_8_REV 0x8367
#endif
#ifndef GL_UNSIGNED_INT_2_10_10_10_REV
#define GL_UNSIGNED_INT_2_10_10_10_REV 0x8368
#endif
#ifndef GL_BGR
#define GL_BGR 0x80E0
#endif
#ifndef GL_BGRA
#define GL_BGRA 0x80E1
#endif
#ifndef GL_MAX_ELEMENTS_VERTICES
#define GL_MAX_ELEMENTS_VERTICES 0x80E8
#endif
#ifndef GL_MAX_ELEMENTS_INDICES
#define GL_MAX_ELEMENTS_INDICES 0x80E9
#endif
#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif
#ifndef GL_TEXTURE_MIN_LOD
#define GL_TEXTURE_MIN_LOD 0x813A
#endif
#ifndef GL_TEXTURE_MAX_LOD
#define GL_TEXTURE_MAX_LOD 0x813B
#endif
#ifndef GL_TEXTURE_BASE_LEVEL
#define GL_TEXTURE_BASE_LEVEL 0x813C
#endif
#ifndef GL_TEXTURE_MAX_LEVEL
#define GL_TEXTURE_MAX_LEVEL 0x813D
#endif
#ifndef GL_LIGHT_MODEL_COLOR_CONTROL
#define GL_LIGHT_MODEL_COLOR_CONTROL 0x81F8
#endif
#ifndef GL_SINGLE_COLOR
#define GL_SINGLE_COLOR 0x81F9
#endif
#ifndef GL_SEPARATE_SPECULAR_COLOR
#define GL_SEPARATE_SPECULAR_COLOR 0x81FA
#endif
#ifndef GL_SMOOTH_POINT_SIZE_RANGE
#define GL_SMOOTH_POINT_SIZE_RANGE 0x0B12
#endif
#ifndef GL_SMOOTH_POINT_SIZE_GRANULARITY
#define GL_SMOOTH_POINT_SIZE_GRANULARITY 0x0B13
#endif
#ifndef GL_SMOOTH_LINE_WIDTH_RANGE
#define GL_SMOOTH_LINE_WIDTH_RANGE 0x0B22
#endif
#ifndef GL_SMOOTH_LINE_WIDTH_GRANULARITY
#define GL_SMOOTH_LINE_WIDTH_GRANULARITY 0x0B23
#endif
#ifndef GL_ALIASED_POINT_SIZE_RANGE
#define GL_ALIASED_POINT_SIZE_RANGE 0x846D
#endif
#ifndef GL_ALIASED_LINE_WIDTH_RANGE
#define GL_ALIASED_LINE_WIDTH_RANGE 0x846E
#endif
#ifndef GL_CONSTANT_COLOR
#define GL_CONSTANT_COLOR 0x8001
#endif
#ifndef GL_ONE_MINUS_CONSTANT_COLOR
#define GL_ONE_MINUS_CONSTANT_COLOR 0x8002
#endif
#ifndef GL_CONSTANT_ALPHA
#define GL_CONSTANT_ALPHA 0x8003
#endif
#ifndef GL_ONE_MINUS_CONSTANT_ALPHA
#define GL_ONE_MINUS_CONSTANT_ALPHA 0x8004
#endif
#ifndef GL_BLEND_COLOR
#define GL_BLEND_COLOR 0x8005
#endif
#ifndef GL_FUNC_ADD
#define GL_FUNC_ADD 0x8006
#endif
#ifndef GL_MIN
#define GL_MIN 0x8007
#endif
#ifndef GL_MAX
#define GL_MAX 0x8008
#endif
#ifndef GL_BLEND_EQUATION
#define GL_BLEND_EQUATION 0x8009
#endif
#ifndef GL_FUNC_SUBTRACT
#define GL_FUNC_SUBTRACT 0x800A
#endif
#ifndef GL_FUNC_REVERSE_SUBTRACT
#define GL_FUNC_REVERSE_SUBTRACT 0x800B
#endif
#ifndef GL_CONVOLUTION_1D
#define GL_CONVOLUTION_1D 0x8010
#endif
#ifndef GL_CONVOLUTION_2D
#define GL_CONVOLUTION_2D 0x8011
#endif
#ifndef GL_SEPARABLE_2D
#define GL_SEPARABLE_2D 0x8012
#endif
#ifndef GL_CONVOLUTION_BORDER_MODE
#define GL_CONVOLUTION_BORDER_MODE 0x8013
#endif
#ifndef GL_CONVOLUTION_FILTER_SCALE
#define GL_CONVOLUTION_FILTER_SCALE 0x8014
#endif
#ifndef GL_CONVOLUTION_FILTER_BIAS
#define GL_CONVOLUTION_FILTER_BIAS 0x8015
#endif
#ifndef GL_REDUCE
#define GL_REDUCE 0x8016
#endif
#ifndef GL_CONVOLUTION_FORMAT
#define GL_CONVOLUTION_FORMAT 0x8017
#endif
#ifndef GL_CONVOLUTION_WIDTH
#define GL_CONVOLUTION_WIDTH 0x8018
#endif
#ifndef GL_CONVOLUTION_HEIGHT
#define GL_CONVOLUTION_HEIGHT 0x8019
#endif
#ifndef GL_MAX_CONVOLUTION_WIDTH
#define GL_MAX_CONVOLUTION_WIDTH 0x801A
#endif
#ifndef GL_MAX_CONVOLUTION_HEIGHT
#define GL_MAX_CONVOLUTION_HEIGHT 0x801B
#endif
#ifndef GL_POST_CONVOLUTION_RED_SCALE
#define GL_POST_CONVOLUTION_RED_SCALE 0x801C
#endif
#ifndef GL_POST_CONVOLUTION_GREEN_SCALE
#define GL_POST_CONVOLUTION_GREEN_SCALE 0x801D
#endif
#ifndef GL_POST_CONVOLUTION_BLUE_SCALE
#define GL_POST_CONVOLUTION_BLUE_SCALE 0x801E
#endif
#ifndef GL_POST_CONVOLUTION_ALPHA_SCALE
#define GL_POST_CONVOLUTION_ALPHA_SCALE 0x801F
#endif
#ifndef GL_POST_CONVOLUTION_RED_BIAS
#define GL_POST_CONVOLUTION_RED_BIAS 0x8020
#endif
#ifndef GL_POST_CONVOLUTION_GREEN_BIAS
#define GL_POST_CONVOLUTION_GREEN_BIAS 0x8021
#endif
#ifndef GL_POST_CONVOLUTION_BLUE_BIAS
#define GL_POST_CONVOLUTION_BLUE_BIAS 0x8022
#endif
#ifndef GL_POST_CONVOLUTION_ALPHA_BIAS
#define GL_POST_CONVOLUTION_ALPHA_BIAS 0x8023
#endif
#ifndef GL_HISTOGRAM
#define GL_HISTOGRAM 0x8024
#endif
#ifndef GL_PROXY_HISTOGRAM
#define GL_PROXY_HISTOGRAM 0x8025
#endif
#ifndef GL_HISTOGRAM_WIDTH
#define GL_HISTOGRAM_WIDTH 0x8026
#endif
#ifndef GL_HISTOGRAM_FORMAT
#define GL_HISTOGRAM_FORMAT 0x8027
#endif
#ifndef GL_HISTOGRAM_RED_SIZE
#define GL_HISTOGRAM_RED_SIZE 0x8028
#endif
#ifndef GL_HISTOGRAM_GREEN_SIZE
#define GL_HISTOGRAM_GREEN_SIZE 0x8029
#endif
#ifndef GL_HISTOGRAM_BLUE_SIZE
#define GL_HISTOGRAM_BLUE_SIZE 0x802A
#endif
#ifndef GL_HISTOGRAM_ALPHA_SIZE
#define GL_HISTOGRAM_ALPHA_SIZE 0x802B
#endif
#ifndef GL_HISTOGRAM_LUMINANCE_SIZE
#define GL_HISTOGRAM_LUMINANCE_SIZE 0x802C
#endif
#ifndef GL_HISTOGRAM_SINK
#define GL_HISTOGRAM_SINK 0x802D
#endif
#ifndef GL_MINMAX
#define GL_MINMAX 0x802E
#endif
#ifndef GL_MINMAX_FORMAT
#define GL_MINMAX_FORMAT 0x802F
#endif
#ifndef GL_MINMAX_SINK
#define GL_MINMAX_SINK 0x8030
#endif
#ifndef GL_TABLE_TOO_LARGE
#define GL_TABLE_TOO_LARGE 0x8031
#endif
#ifndef GL_COLOR_MATRIX
#define GL_COLOR_MATRIX 0x80B1
#endif
#ifndef GL_COLOR_MATRIX_STACK_DEPTH
#define GL_COLOR_MATRIX_STACK_DEPTH 0x80B2
#endif
#ifndef GL_MAX_COLOR_MATRIX_STACK_DEPTH
#define GL_MAX_COLOR_MATRIX_STACK_DEPTH 0x80B3
#endif
#ifndef GL_POST_COLOR_MATRIX_RED_SCALE
#define GL_POST_COLOR_MATRIX_RED_SCALE 0x80B4
#endif
#ifndef GL_POST_COLOR_MATRIX_GREEN_SCALE
#define GL_POST_COLOR_MATRIX_GREEN_SCALE 0x80B5
#endif
#ifndef GL_POST_COLOR_MATRIX_BLUE_SCALE
#define GL_POST_COLOR_MATRIX_BLUE_SCALE 0x80B6
#endif
#ifndef GL_POST_COLOR_MATRIX_ALPHA_SCALE
#define GL_POST_COLOR_MATRIX_ALPHA_SCALE 0x80B7
#endif
#ifndef GL_POST_COLOR_MATRIX_RED_BIAS
#define GL_POST_COLOR_MATRIX_RED_BIAS 0x80B8
#endif
#ifndef GL_POST_COLOR_MATRIX_GREEN_BIAS
#define GL_POST_COLOR_MATRIX_GREEN_BIAS 0x80B9
#endif
#ifndef GL_POST_COLOR_MATRIX_BLUE_BIAS
#define GL_POST_COLOR_MATRIX_BLUE_BIAS 0x80BA
#endif
#ifndef GL_POST_COLOR_MATRIX_ALPHA_BIAS
#define GL_POST_COLOR_MATRIX_ALPHA_BIAS 0x80BB
#endif
#ifndef GL_COLOR_TABLE
#define GL_COLOR_TABLE 0x80D0
#endif
#ifndef GL_POST_CONVOLUTION_COLOR_TABLE
#define GL_POST_CONVOLUTION_COLOR_TABLE 0x80D1
#endif
#ifndef GL_POST_COLOR_MATRIX_COLOR_TABLE
#define GL_POST_COLOR_MATRIX_COLOR_TABLE 0x80D2
#endif
#ifndef GL_PROXY_COLOR_TABLE
#define GL_PROXY_COLOR_TABLE 0x80D3
#endif
#ifndef GL_PROXY_POST_CONVOLUTION_COLOR_TABLE
#define GL_PROXY_POST_CONVOLUTION_COLOR_TABLE 0x80D4
#endif
#ifndef GL_PROXY_POST_COLOR_MATRIX_COLOR_TABLE
#define GL_PROXY_POST_COLOR_MATRIX_COLOR_TABLE 0x80D5
#endif
#ifndef GL_COLOR_TABLE_SCALE
#define GL_COLOR_TABLE_SCALE 0x80D6
#endif
#ifndef GL_COLOR_TABLE_BIAS
#define GL_COLOR_TABLE_BIAS 0x80D7
#endif
#ifndef GL_COLOR_TABLE_FORMAT
#define GL_COLOR_TABLE_FORMAT 0x80D8
#endif
#ifndef GL_COLOR_TABLE_WIDTH
#define GL_COLOR_TABLE_WIDTH 0x80D9
#endif
#ifndef GL_COLOR_TABLE_RED_SIZE
#define GL_COLOR_TABLE_RED_SIZE 0x80DA
#endif
#ifndef GL_COLOR_TABLE_GREEN_SIZE
#define GL_COLOR_TABLE_GREEN_SIZE 0x80DB
#endif
#ifndef GL_COLOR_TABLE_BLUE_SIZE
#define GL_COLOR_TABLE_BLUE_SIZE 0x80DC
#endif
#ifndef GL_COLOR_TABLE_ALPHA_SIZE
#define GL_COLOR_TABLE_ALPHA_SIZE 0x80DD
#endif
#ifndef GL_COLOR_TABLE_LUMINANCE_SIZE
#define GL_COLOR_TABLE_LUMINANCE_SIZE 0x80DE
#endif
#ifndef GL_COLOR_TABLE_INTENSITY_SIZE
#define GL_COLOR_TABLE_INTENSITY_SIZE 0x80DF
#endif
#ifndef GL_CONSTANT_BORDER
#define GL_CONSTANT_BORDER 0x8151
#endif
#ifndef GL_REPLICATE_BORDER
#define GL_REPLICATE_BORDER 0x8153
#endif
#ifndef GL_CONVOLUTION_BORDER_COLOR
#define GL_CONVOLUTION_BORDER_COLOR 0x8154
#endif
#ifndef GL_TEXTURE0
#define GL_TEXTURE0 0x84C0
#endif
#ifndef GL_TEXTURE1
#define GL_TEXTURE1 0x84C1
#endif
#ifndef GL_TEXTURE2
#define GL_TEXTURE2 0x84C2
#endif
#ifndef GL_TEXTURE3
#define GL_TEXTURE3 0x84C3
#endif
#ifndef GL_TEXTURE4
#define GL_TEXTURE4 0x84C4
#endif
#ifndef GL_TEXTURE5
#define GL_TEXTURE5 0x84C5
#endif
#ifndef GL_TEXTURE6
#define GL_TEXTURE6 0x84C6
#endif
#ifndef GL_TEXTURE7
#define GL_TEXTURE7 0x84C7
#endif
#ifndef GL_TEXTURE8
#define GL_TEXTURE8 0x84C8
#endif
#ifndef GL_TEXTURE9
#define GL_TEXTURE9 0x84C9
#endif
#ifndef GL_TEXTURE10
#define GL_TEXTURE10 0x84CA
#endif
#ifndef GL_TEXTURE11
#define GL_TEXTURE11 0x84CB
#endif
#ifndef GL_TEXTURE12
#define GL_TEXTURE12 0x84CC
#endif
#ifndef GL_TEXTURE13
#define GL_TEXTURE13 0x84CD
#endif
#ifndef GL_TEXTURE14
#define GL_TEXTURE14 0x84CE
#endif
#ifndef GL_TEXTURE15
#define GL_TEXTURE15 0x84CF
#endif
#ifndef GL_TEXTURE16
#define GL_TEXTURE16 0x84D0
#endif
#ifndef GL_TEXTURE17
#define GL_TEXTURE17 0x84D1
#endif
#ifndef GL_TEXTURE18
#define GL_TEXTURE18 0x84D2
#endif
#ifndef GL_TEXTURE19
#define GL_TEXTURE19 0x84D3
#endif
#ifndef GL_TEXTURE20
#define GL_TEXTURE20 0x84D4
#endif
#ifndef GL_TEXTURE21
#define GL_TEXTURE21 0x84D5
#endif
#ifndef GL_TEXTURE22
#define GL_TEXTURE22 0x84D6
#endif
#ifndef GL_TEXTURE23
#define GL_TEXTURE23 0x84D7
#endif
#ifndef GL_TEXTURE24
#define GL_TEXTURE24 0x84D8
#endif
#ifndef GL_TEXTURE25
#define GL_TEXTURE25 0x84D9
#endif
#ifndef GL_TEXTURE26
#define GL_TEXTURE26 0x84DA
#endif
#ifndef GL_TEXTURE27
#define GL_TEXTURE27 0x84DB
#endif
#ifndef GL_TEXTURE28
#define GL_TEXTURE28 0x84DC
#endif
#ifndef GL_TEXTURE29
#define GL_TEXTURE29 0x84DD
#endif
#ifndef GL_TEXTURE30
#define GL_TEXTURE30 0x84DE
#endif
#ifndef GL_TEXTURE31
#define GL_TEXTURE31 0x84DF
#endif
#ifndef GL_ACTIVE_TEXTURE
#define GL_ACTIVE_TEXTURE 0x84E0
#endif
#ifndef GL_CLIENT_ACTIVE_TEXTURE
#define GL_CLIENT_ACTIVE_TEXTURE 0x84E1
#endif
#ifndef GL_MAX_TEXTURE_UNITS
#define GL_MAX_TEXTURE_UNITS 0x84E2
#endif
#ifndef GL_TRANSPOSE_MODELVIEW_MATRIX
#define GL_TRANSPOSE_MODELVIEW_MATRIX 0x84E3
#endif
#ifndef GL_TRANSPOSE_PROJECTION_MATRIX
#define GL_TRANSPOSE_PROJECTION_MATRIX 0x84E4
#endif
#ifndef GL_TRANSPOSE_TEXTURE_MATRIX
#define GL_TRANSPOSE_TEXTURE_MATRIX 0x84E5
#endif
#ifndef GL_TRANSPOSE_COLOR_MATRIX
#define GL_TRANSPOSE_COLOR_MATRIX 0x84E6
#endif
#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE 0x809D
#endif
#ifndef GL_SAMPLE_ALPHA_TO_COVERAGE
#define GL_SAMPLE_ALPHA_TO_COVERAGE 0x809E
#endif
#ifndef GL_SAMPLE_ALPHA_TO_ONE
#define GL_SAMPLE_ALPHA_TO_ONE 0x809F
#endif
#ifndef GL_SAMPLE_COVERAGE
#define GL_SAMPLE_COVERAGE 0x80A0
#endif
#ifndef GL_SAMPLE_BUFFERS
#define GL_SAMPLE_BUFFERS 0x80A8
#endif
#ifndef GL_SAMPLES
#define GL_SAMPLES 0x80A9
#endif
#ifndef GL_SAMPLE_COVERAGE_VALUE
#define GL_SAMPLE_COVERAGE_VALUE 0x80AA
#endif
#ifndef GL_SAMPLE_COVERAGE_INVERT
#define GL_SAMPLE_COVERAGE_INVERT 0x80AB
#endif
#ifndef GL_MULTISAMPLE_BIT
#define GL_MULTISAMPLE_BIT 0x20000000
#endif
#ifndef GL_NORMAL_MAP
#define GL_NORMAL_MAP 0x8511
#endif
#ifndef GL_REFLECTION_MAP
#define GL_REFLECTION_MAP 0x8512
#endif
#ifndef GL_TEXTURE_CUBE_MAP
#define GL_TEXTURE_CUBE_MAP 0x8513
#endif
#ifndef GL_TEXTURE_BINDING_CUBE_MAP
#define GL_TEXTURE_BINDING_CUBE_MAP 0x8514
#endif
#ifndef GL_TEXTURE_CUBE_MAP_POSITIVE_X
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X 0x8515
#endif
#ifndef GL_TEXTURE_CUBE_MAP_NEGATIVE_X
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X 0x8516
#endif
#ifndef GL_TEXTURE_CUBE_MAP_POSITIVE_Y
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y 0x8517
#endif
#ifndef GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y 0x8518
#endif
#ifndef GL_TEXTURE_CUBE_MAP_POSITIVE_Z
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z 0x8519
#endif
#ifndef GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z 0x851A
#endif
#ifndef GL_PROXY_TEXTURE_CUBE_MAP
#define GL_PROXY_TEXTURE_CUBE_MAP 0x851B
#endif
#ifndef GL_MAX_CUBE_MAP_TEXTURE_SIZE
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE 0x851C
#endif
#ifndef GL_COMPRESSED_ALPHA
#define GL_COMPRESSED_ALPHA 0x84E9
#endif
#ifndef GL_COMPRESSED_LUMINANCE
#define GL_COMPRESSED_LUMINANCE 0x84EA
#endif
#ifndef GL_COMPRESSED_LUMINANCE_ALPHA
#define GL_COMPRESSED_LUMINANCE_ALPHA 0x84EB
#endif
#ifndef GL_COMPRESSED_INTENSITY
#define GL_COMPRESSED_INTENSITY 0x84EC
#endif
#ifndef GL_COMPRESSED_RGB
#define GL_COMPRESSED_RGB 0x84ED
#endif
#ifndef GL_COMPRESSED_RGBA
#define GL_COMPRESSED_RGBA 0x84EE
#endif
#ifndef GL_TEXTURE_COMPRESSION_HINT
#define GL_TEXTURE_COMPRESSION_HINT 0x84EF
#endif
#ifndef GL_TEXTURE_COMPRESSED_IMAGE_SIZE
#define GL_TEXTURE_COMPRESSED_IMAGE_SIZE 0x86A0
#endif
#ifndef GL_TEXTURE_COMPRESSED
#define GL_TEXTURE_COMPRESSED 0x86A1
#endif
#ifndef GL_NUM_COMPRESSED_TEXTURE_FORMATS
#define GL_NUM_COMPRESSED_TEXTURE_FORMATS 0x86A2
#endif
#ifndef GL_COMPRESSED_TEXTURE_FORMATS
#define GL_COMPRESSED_TEXTURE_FORMATS 0x86A3
#endif
#ifndef GL_CLAMP_TO_BORDER
#define GL_CLAMP_TO_BORDER 0x812D
#endif
#ifndef GL_COMBINE
#define GL_COMBINE 0x8570
#endif
#ifndef GL_COMBINE_RGB
#define GL_COMBINE_RGB 0x8571
#endif
#ifndef GL_COMBINE_ALPHA
#define GL_COMBINE_ALPHA 0x8572
#endif
#ifndef GL_SOURCE0_RGB
#define GL_SOURCE0_RGB 0x8580
#endif
#ifndef GL_SOURCE1_RGB
#define GL_SOURCE1_RGB 0x8581
#endif
#ifndef GL_SOURCE2_RGB
#define GL_SOURCE2_RGB 0x8582
#endif
#ifndef GL_SOURCE0_ALPHA
#define GL_SOURCE0_ALPHA 0x8588
#endif
#ifndef GL_SOURCE1_ALPHA
#define GL_SOURCE1_ALPHA 0x8589
#endif
#ifndef GL_SOURCE2_ALPHA
#define GL_SOURCE2_ALPHA 0x858A
#endif
#ifndef GL_OPERAND0_RGB
#define GL_OPERAND0_RGB 0x8590
#endif
#ifndef GL_OPERAND1_RGB
#define GL_OPERAND1_RGB 0x8591
#endif
#ifndef GL_OPERAND2_RGB
#define GL_OPERAND2_RGB 0x8592
#endif
#ifndef GL_OPERAND0_ALPHA
#define GL_OPERAND0_ALPHA 0x8598
#endif
#ifndef GL_OPERAND1_ALPHA
#define GL_OPERAND1_ALPHA 0x8599
#endif
#ifndef GL_OPERAND2_ALPHA
#define GL_OPERAND2_ALPHA 0x859A
#endif
#ifndef GL_RGB_SCALE
#define GL_RGB_SCALE 0x8573
#endif
#ifndef GL_ADD_SIGNED
#define GL_ADD_SIGNED 0x8574
#endif
#ifndef GL_INTERPOLATE
#define GL_INTERPOLATE 0x8575
#endif
#ifndef GL_SUBTRACT
#define GL_SUBTRACT 0x84E7
#endif
#ifndef GL_CONSTANT
#define GL_CONSTANT 0x8576
#endif
#ifndef GL_PRIMARY_COLOR
#define GL_PRIMARY_COLOR 0x8577
#endif
#ifndef GL_PREVIOUS
#define GL_PREVIOUS 0x8578
#endif
#ifndef GL_DOT3_RGB
#define GL_DOT3_RGB 0x86AE
#endif
#ifndef GL_DOT3_RGBA
#define GL_DOT3_RGBA 0x86AF
#endif
#ifndef GL_BLEND_DST_RGB
#define GL_BLEND_DST_RGB 0x80C8
#endif
#ifndef GL_BLEND_SRC_RGB
#define GL_BLEND_SRC_RGB 0x80C9
#endif
#ifndef GL_BLEND_DST_ALPHA
#define GL_BLEND_DST_ALPHA 0x80CA
#endif
#ifndef GL_BLEND_SRC_ALPHA
#define GL_BLEND_SRC_ALPHA 0x80CB
#endif
#ifndef GL_POINT_SIZE_MIN
#define GL_POINT_SIZE_MIN 0x8126
#endif
#ifndef GL_POINT_SIZE_MAX
#define GL_POINT_SIZE_MAX 0x8127
#endif
#ifndef GL_POINT_FADE_THRESHOLD_SIZE
#define GL_POINT_FADE_THRESHOLD_SIZE 0x8128
#endif
#ifndef GL_POINT_DISTANCE_ATTENUATION
#define GL_POINT_DISTANCE_ATTENUATION 0x8129
#endif
#ifndef GL_GENERATE_MIPMAP
#define GL_GENERATE_MIPMAP 0x8191
#endif
#ifndef GL_GENERATE_MIPMAP_HINT
#define GL_GENERATE_MIPMAP_HINT 0x8192
#endif
#ifndef GL_DEPTH_COMPONENT16
#define GL_DEPTH_COMPONENT16 0x81A5
#endif
#ifndef GL_DEPTH_COMPONENT24
#define GL_DEPTH_COMPONENT24 0x81A6
#endif
#ifndef GL_DEPTH_COMPONENT32
#define GL_DEPTH_COMPONENT32 0x81A7
#endif
#ifndef GL_MIRRORED_REPEAT
#define GL_MIRRORED_REPEAT 0x8370
#endif
#ifndef GL_FOG_COORDINATE_SOURCE
#define GL_FOG_COORDINATE_SOURCE 0x8450
#endif
#ifndef GL_FOG_COORDINATE
#define GL_FOG_COORDINATE 0x8451
#endif
#ifndef GL_FRAGMENT_DEPTH
#define GL_FRAGMENT_DEPTH 0x8452
#endif
#ifndef GL_CURRENT_FOG_COORDINATE
#define GL_CURRENT_FOG_COORDINATE 0x8453
#endif
#ifndef GL_FOG_COORDINATE_ARRAY_TYPE
#define GL_FOG_COORDINATE_ARRAY_TYPE 0x8454
#endif
#ifndef GL_FOG_COORDINATE_ARRAY_STRIDE
#define GL_FOG_COORDINATE_ARRAY_STRIDE 0x8455
#endif
#ifndef GL_FOG_COORDINATE_ARRAY_POINTER
#define GL_FOG_COORDINATE_ARRAY_POINTER 0x8456
#endif
#ifndef GL_FOG_COORDINATE_ARRAY
#define GL_FOG_COORDINATE_ARRAY 0x8457
#endif
#ifndef GL_COLOR_SUM
#define GL_COLOR_SUM 0x8458
#endif
#ifndef GL_CURRENT_SECONDARY_COLOR
#define GL_CURRENT_SECONDARY_COLOR 0x8459
#endif
#ifndef GL_SECONDARY_COLOR_ARRAY_SIZE
#define GL_SECONDARY_COLOR_ARRAY_SIZE 0x845A
#endif
#ifndef GL_SECONDARY_COLOR_ARRAY_TYPE
#define GL_SECONDARY_COLOR_ARRAY_TYPE 0x845B
#endif
#ifndef GL_SECONDARY_COLOR_ARRAY_STRIDE
#define GL_SECONDARY_COLOR_ARRAY_STRIDE 0x845C
#endif
#ifndef GL_SECONDARY_COLOR_ARRAY_POINTER
#define GL_SECONDARY_COLOR_ARRAY_POINTER 0x845D
#endif
#ifndef GL_SECONDARY_COLOR_ARRAY
#define GL_SECONDARY_COLOR_ARRAY 0x845E
#endif
#ifndef GL_MAX_TEXTURE_LOD_BIAS
#define GL_MAX_TEXTURE_LOD_BIAS 0x84FD
#endif
#ifndef GL_TEXTURE_FILTER_CONTROL
#define GL_TEXTURE_FILTER_CONTROL 0x8500
#endif
#ifndef GL_TEXTURE_LOD_BIAS
#define GL_TEXTURE_LOD_BIAS 0x8501
#endif
#ifndef GL_INCR_WRAP
#define GL_INCR_WRAP 0x8507
#endif
#ifndef GL_DECR_WRAP
#define GL_DECR_WRAP 0x8508
#endif
#ifndef GL_TEXTURE_DEPTH_SIZE
#define GL_TEXTURE_DEPTH_SIZE 0x884A
#endif
#ifndef GL_DEPTH_TEXTURE_MODE
#define GL_DEPTH_TEXTURE_MODE 0x884B
#endif
#ifndef GL_TEXTURE_COMPARE_MODE
#define GL_TEXTURE_COMPARE_MODE 0x884C
#endif
#ifndef GL_TEXTURE_COMPARE_FUNC
#define GL_TEXTURE_COMPARE_FUNC 0x884D
#endif
#ifndef GL_COMPARE_R_TO_TEXTURE
#define GL_COMPARE_R_TO_TEXTURE 0x884E
#endif
#ifndef GL_BUFFER_SIZE
#define GL_BUFFER_SIZE 0x8764
#endif
#ifndef GL_BUFFER_USAGE
#define GL_BUFFER_USAGE 0x8765
#endif
#ifndef GL_QUERY_COUNTER_BITS
#define GL_QUERY_COUNTER_BITS 0x8864
#endif
#ifndef GL_CURRENT_QUERY
#define GL_CURRENT_QUERY 0x8865
#endif
#ifndef GL_QUERY_RESULT
#define GL_QUERY_RESULT 0x8866
#endif
#ifndef GL_QUERY_RESULT_AVAILABLE
#define GL_QUERY_RESULT_AVAILABLE 0x8867
#endif
#ifndef GL_ARRAY_BUFFER
#define GL_ARRAY_BUFFER 0x8892
#endif
#ifndef GL_ELEMENT_ARRAY_BUFFER
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#endif
#ifndef GL_ARRAY_BUFFER_BINDING
#define GL_ARRAY_BUFFER_BINDING 0x8894
#endif
#ifndef GL_ELEMENT_ARRAY_BUFFER_BINDING
#define GL_ELEMENT_ARRAY_BUFFER_BINDING 0x8895
#endif
#ifndef GL_VERTEX_ARRAY_BUFFER_BINDING
#define GL_VERTEX_ARRAY_BUFFER_BINDING 0x8896
#endif
#ifndef GL_NORMAL_ARRAY_BUFFER_BINDING
#define GL_NORMAL_ARRAY_BUFFER_BINDING 0x8897
#endif
#ifndef GL_COLOR_ARRAY_BUFFER_BINDING
#define GL_COLOR_ARRAY_BUFFER_BINDING 0x8898
#endif
#ifndef GL_INDEX_ARRAY_BUFFER_BINDING
#define GL_INDEX_ARRAY_BUFFER_BINDING 0x8899
#endif
#ifndef GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING
#define GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING 0x889A
#endif
#ifndef GL_EDGE_FLAG_ARRAY_BUFFER_BINDING
#define GL_EDGE_FLAG_ARRAY_BUFFER_BINDING 0x889B
#endif
#ifndef GL_SECONDARY_COLOR_ARRAY_BUFFER_BINDING
#define GL_SECONDARY_COLOR_ARRAY_BUFFER_BINDING 0x889C
#endif
#ifndef GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING
#define GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING 0x889D
#endif
#ifndef GL_WEIGHT_ARRAY_BUFFER_BINDING
#define GL_WEIGHT_ARRAY_BUFFER_BINDING 0x889E
#endif
#ifndef GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING
#define GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING 0x889F
#endif
#ifndef GL_READ_ONLY
#define GL_READ_ONLY 0x88B8
#endif
#ifndef GL_WRITE_ONLY
#define GL_WRITE_ONLY 0x88B9
#endif
#ifndef GL_READ_WRITE
#define GL_READ_WRITE 0x88BA
#endif
#ifndef GL_BUFFER_ACCESS
#define GL_BUFFER_ACCESS 0x88BB
#endif
#ifndef GL_BUFFER_MAPPED
#define GL_BUFFER_MAPPED 0x88BC
#endif
#ifndef GL_BUFFER_MAP_POINTER
#define GL_BUFFER_MAP_POINTER 0x88BD
#endif
#ifndef GL_STREAM_DRAW
#define GL_STREAM_DRAW 0x88E0
#endif
#ifndef GL_STREAM_READ
#define GL_STREAM_READ 0x88E1
#endif
#ifndef GL_STREAM_COPY
#define GL_STREAM_COPY 0x88E2
#endif
#ifndef GL_STATIC_DRAW
#define GL_STATIC_DRAW 0x88E4
#endif
#ifndef GL_STATIC_READ
#define GL_STATIC_READ 0x88E5
#endif
#ifndef GL_STATIC_COPY
#define GL_STATIC_COPY 0x88E6
#endif
#ifndef GL_DYNAMIC_DRAW
#define GL_DYNAMIC_DRAW 0x88E8
#endif
#ifndef GL_DYNAMIC_READ
#define GL_DYNAMIC_READ 0x88E9
#endif
#ifndef GL_DYNAMIC_COPY
#define GL_DYNAMIC_COPY 0x88EA
#endif
#ifndef GL_SAMPLES_PASSED
#define GL_SAMPLES_PASSED 0x8914
#endif
#ifndef GL_FOG_COORD_SRC
#define GL_FOG_COORD_SRC GL_FOG_COORDINATE_SOURCE
#endif
#ifndef GL_FOG_COORD
#define GL_FOG_COORD GL_FOG_COORDINATE
#endif
#ifndef GL_CURRENT_FOG_COORD
#define GL_CURRENT_FOG_COORD GL_CURRENT_FOG_COORDINATE
#endif
#ifndef GL_FOG_COORD_ARRAY_TYPE
#define GL_FOG_COORD_ARRAY_TYPE GL_FOG_COORDINATE_ARRAY_TYPE
#endif
#ifndef GL_FOG_COORD_ARRAY_STRIDE
#define GL_FOG_COORD_ARRAY_STRIDE GL_FOG_COORDINATE_ARRAY_STRIDE
#endif
#ifndef GL_FOG_COORD_ARRAY_POINTER
#define GL_FOG_COORD_ARRAY_POINTER GL_FOG_COORDINATE_ARRAY_POINTER
#endif
#ifndef GL_FOG_COORD_ARRAY
#define GL_FOG_COORD_ARRAY GL_FOG_COORDINATE_ARRAY
#endif
#ifndef GL_FOG_COORD_ARRAY_BUFFER_BINDING
#define GL_FOG_COORD_ARRAY_BUFFER_BINDING GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING
#endif
#ifndef GL_SRC0_RGB
#define GL_SRC0_RGB GL_SOURCE0_RGB
#endif
#ifndef GL_SRC1_RGB
#define GL_SRC1_RGB GL_SOURCE1_RGB
#endif
#ifndef GL_SRC2_RGB
#define GL_SRC2_RGB GL_SOURCE2_RGB
#endif
#ifndef GL_SRC0_ALPHA
#define GL_SRC0_ALPHA GL_SOURCE0_ALPHA
#endif
#ifndef GL_SRC1_ALPHA
#define GL_SRC1_ALPHA GL_SOURCE1_ALPHA
#endif
#ifndef GL_SRC2_ALPHA
#define GL_SRC2_ALPHA GL_SOURCE2_ALPHA
#endif
#ifndef GL_BLEND_EQUATION_RGB
#define GL_BLEND_EQUATION_RGB GL_BLEND_EQUATION
#endif
#ifndef GL_VERTEX_ATTRIB_ARRAY_ENABLED
#define GL_VERTEX_ATTRIB_ARRAY_ENABLED 0x8622
#endif
#ifndef GL_VERTEX_ATTRIB_ARRAY_SIZE
#define GL_VERTEX_ATTRIB_ARRAY_SIZE 0x8623
#endif
#ifndef GL_VERTEX_ATTRIB_ARRAY_STRIDE
#define GL_VERTEX_ATTRIB_ARRAY_STRIDE 0x8624
#endif
#ifndef GL_VERTEX_ATTRIB_ARRAY_TYPE
#define GL_VERTEX_ATTRIB_ARRAY_TYPE 0x8625
#endif
#ifndef GL_CURRENT_VERTEX_ATTRIB
#define GL_CURRENT_VERTEX_ATTRIB 0x8626
#endif
#ifndef GL_VERTEX_PROGRAM_POINT_SIZE
#define GL_VERTEX_PROGRAM_POINT_SIZE 0x8642
#endif
#ifndef GL_VERTEX_PROGRAM_TWO_SIDE
#define GL_VERTEX_PROGRAM_TWO_SIDE 0x8643
#endif
#ifndef GL_VERTEX_ATTRIB_ARRAY_POINTER
#define GL_VERTEX_ATTRIB_ARRAY_POINTER 0x8645
#endif
#ifndef GL_STENCIL_BACK_FUNC
#define GL_STENCIL_BACK_FUNC 0x8800
#endif
#ifndef GL_STENCIL_BACK_FAIL
#define GL_STENCIL_BACK_FAIL 0x8801
#endif
#ifndef GL_STENCIL_BACK_PASS_DEPTH_FAIL
#define GL_STENCIL_BACK_PASS_DEPTH_FAIL 0x8802
#endif
#ifndef GL_STENCIL_BACK_PASS_DEPTH_PASS
#define GL_STENCIL_BACK_PASS_DEPTH_PASS 0x8803
#endif
#ifndef GL_MAX_DRAW_BUFFERS
#define GL_MAX_DRAW_BUFFERS 0x8824
#endif
#ifndef GL_DRAW_BUFFER0
#define GL_DRAW_BUFFER0 0x8825
#endif
#ifndef GL_DRAW_BUFFER1
#define GL_DRAW_BUFFER1 0x8826
#endif
#ifndef GL_DRAW_BUFFER2
#define GL_DRAW_BUFFER2 0x8827
#endif
#ifndef GL_DRAW_BUFFER3
#define GL_DRAW_BUFFER3 0x8828
#endif
#ifndef GL_DRAW_BUFFER4
#define GL_DRAW_BUFFER4 0x8829
#endif
#ifndef GL_DRAW_BUFFER5
#define GL_DRAW_BUFFER5 0x882A
#endif
#ifndef GL_DRAW_BUFFER6
#define GL_DRAW_BUFFER6 0x882B
#endif
#ifndef GL_DRAW_BUFFER7
#define GL_DRAW_BUFFER7 0x882C
#endif
#ifndef GL_DRAW_BUFFER8
#define GL_DRAW_BUFFER8 0x882D
#endif
#ifndef GL_DRAW_BUFFER9
#define GL_DRAW_BUFFER9 0x882E
#endif
#ifndef GL_DRAW_BUFFER10
#define GL_DRAW_BUFFER10 0x882F
#endif
#ifndef GL_DRAW_BUFFER11
#define GL_DRAW_BUFFER11 0x8830
#endif
#ifndef GL_DRAW_BUFFER12
#define GL_DRAW_BUFFER12 0x8831
#endif
#ifndef GL_DRAW_BUFFER13
#define GL_DRAW_BUFFER13 0x8832
#endif
#ifndef GL_DRAW_BUFFER14
#define GL_DRAW_BUFFER14 0x8833
#endif
#ifndef GL_DRAW_BUFFER15
#define GL_DRAW_BUFFER15 0x8834
#endif
#ifndef GL_BLEND_EQUATION_ALPHA
#define GL_BLEND_EQUATION_ALPHA 0x883D
#endif
#ifndef GL_POINT_SPRITE
#define GL_POINT_SPRITE 0x8861
#endif
#ifndef GL_COORD_REPLACE
#define GL_COORD_REPLACE 0x8862
#endif
#ifndef GL_MAX_VERTEX_ATTRIBS
#define GL_MAX_VERTEX_ATTRIBS 0x8869
#endif
#ifndef GL_VERTEX_ATTRIB_ARRAY_NORMALIZED
#define GL_VERTEX_ATTRIB_ARRAY_NORMALIZED 0x886A
#endif
#ifndef GL_MAX_TEXTURE_COORDS
#define GL_MAX_TEXTURE_COORDS 0x8871
#endif
#ifndef GL_MAX_TEXTURE_IMAGE_UNITS
#define GL_MAX_TEXTURE_IMAGE_UNITS 0x8872
#endif
#ifndef GL_FRAGMENT_SHADER
#define GL_FRAGMENT_SHADER 0x8B30
#endif
#ifndef GL_VERTEX_SHADER
#define GL_VERTEX_SHADER 0x8B31
#endif
#ifndef GL_MAX_FRAGMENT_UNIFORM_COMPONENTS
#define GL_MAX_FRAGMENT_UNIFORM_COMPONENTS 0x8B49
#endif
#ifndef GL_MAX_VERTEX_UNIFORM_COMPONENTS
#define GL_MAX_VERTEX_UNIFORM_COMPONENTS 0x8B4A
#endif
#ifndef GL_MAX_VARYING_FLOATS
#define GL_MAX_VARYING_FLOATS 0x8B4B
#endif
#ifndef GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS
#define GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS 0x8B4C
#endif
#ifndef GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS
#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS 0x8B4D
#endif
#ifndef GL_SHADER_TYPE
#define GL_SHADER_TYPE 0x8B4F
#endif
#ifndef GL_FLOAT_VEC2
#define GL_FLOAT_VEC2 0x8B50
#endif
#ifndef GL_FLOAT_VEC3
#define GL_FLOAT_VEC3 0x8B51
#endif
#ifndef GL_FLOAT_VEC4
#define GL_FLOAT_VEC4 0x8B52
#endif
#ifndef GL_INT_VEC2
#define GL_INT_VEC2 0x8B53
#endif
#ifndef GL_INT_VEC3
#define GL_INT_VEC3 0x8B54
#endif
#ifndef GL_INT_VEC4
#define GL_INT_VEC4 0x8B55
#endif
#ifndef GL_BOOL
#define GL_BOOL 0x8B56
#endif
#ifndef GL_BOOL_VEC2
#define GL_BOOL_VEC2 0x8B57
#endif
#ifndef GL_BOOL_VEC3
#define GL_BOOL_VEC3 0x8B58
#endif
#ifndef GL_BOOL_VEC4
#define GL_BOOL_VEC4 0x8B59
#endif
#ifndef GL_FLOAT_MAT2
#define GL_FLOAT_MAT2 0x8B5A
#endif
#ifndef GL_FLOAT_MAT3
#define GL_FLOAT_MAT3 0x8B5B
#endif
#ifndef GL_FLOAT_MAT4
#define GL_FLOAT_MAT4 0x8B5C
#endif
#ifndef GL_SAMPLER_1D
#define GL_SAMPLER_1D 0x8B5D
#endif
#ifndef GL_SAMPLER_2D
#define GL_SAMPLER_2D 0x8B5E
#endif
#ifndef GL_SAMPLER_3D
#define GL_SAMPLER_3D 0x8B5F
#endif
#ifndef GL_SAMPLER_CUBE
#define GL_SAMPLER_CUBE 0x8B60
#endif
#ifndef GL_SAMPLER_1D_SHADOW
#define GL_SAMPLER_1D_SHADOW 0x8B61
#endif
#ifndef GL_SAMPLER_2D_SHADOW
#define GL_SAMPLER_2D_SHADOW 0x8B62
#endif
#ifndef GL_DELETE_STATUS
#define GL_DELETE_STATUS 0x8B80
#endif
#ifndef GL_COMPILE_STATUS
#define GL_COMPILE_STATUS 0x8B81
#endif
#ifndef GL_LINK_STATUS
#define GL_LINK_STATUS 0x8B82
#endif
#ifndef GL_VALIDATE_STATUS
#define GL_VALIDATE_STATUS 0x8B83
#endif
#ifndef GL_INFO_LOG_LENGTH
#define GL_INFO_LOG_LENGTH 0x8B84
#endif
#ifndef GL_ATTACHED_SHADERS
#define GL_ATTACHED_SHADERS 0x8B85
#endif
#ifndef GL_ACTIVE_UNIFORMS
#define GL_ACTIVE_UNIFORMS 0x8B86
#endif
#ifndef GL_ACTIVE_UNIFORM_MAX_LENGTH
#define GL_ACTIVE_UNIFORM_MAX_LENGTH 0x8B87
#endif
#ifndef GL_SHADER_SOURCE_LENGTH
#define GL_SHADER_SOURCE_LENGTH 0x8B88
#endif
#ifndef GL_ACTIVE_ATTRIBUTES
#define GL_ACTIVE_ATTRIBUTES 0x8B89
#endif
#ifndef GL_ACTIVE_ATTRIBUTE_MAX_LENGTH
#define GL_ACTIVE_ATTRIBUTE_MAX_LENGTH 0x8B8A
#endif
#ifndef GL_FRAGMENT_SHADER_DERIVATIVE_HINT
#define GL_FRAGMENT_SHADER_DERIVATIVE_HINT 0x8B8B
#endif
#ifndef GL_SHADING_LANGUAGE_VERSION
#define GL_SHADING_LANGUAGE_VERSION 0x8B8C
#endif
#ifndef GL_CURRENT_PROGRAM
#define GL_CURRENT_PROGRAM 0x8B8D
#endif
#ifndef GL_POINT_SPRITE_COORD_ORIGIN
#define GL_POINT_SPRITE_COORD_ORIGIN 0x8CA0
#endif
#ifndef GL_LOWER_LEFT
#define GL_LOWER_LEFT 0x8CA1
#endif
#ifndef GL_UPPER_LEFT
#define GL_UPPER_LEFT 0x8CA2
#endif
#ifndef GL_STENCIL_BACK_REF
#define GL_STENCIL_BACK_REF 0x8CA3
#endif
#ifndef GL_STENCIL_BACK_VALUE_MASK
#define GL_STENCIL_BACK_VALUE_MASK 0x8CA4
#endif
#ifndef GL_STENCIL_BACK_WRITEMASK
#define GL_STENCIL_BACK_WRITEMASK 0x8CA5
#endif
#ifndef GL_CURRENT_RASTER_SECONDARY_COLOR
#define GL_CURRENT_RASTER_SECONDARY_COLOR 0x845F
#endif
#ifndef GL_PIXEL_PACK_BUFFER
#define GL_PIXEL_PACK_BUFFER 0x88EB
#endif
#ifndef GL_PIXEL_UNPACK_BUFFER
#define GL_PIXEL_UNPACK_BUFFER 0x88EC
#endif
#ifndef GL_PIXEL_PACK_BUFFER_BINDING
#define GL_PIXEL_PACK_BUFFER_BINDING 0x88ED
#endif
#ifndef GL_PIXEL_UNPACK_BUFFER_BINDING
#define GL_PIXEL_UNPACK_BUFFER_BINDING 0x88EF
#endif
#ifndef GL_FLOAT_MAT2x3
#define GL_FLOAT_MAT2x3 0x8B65
#endif
#ifndef GL_FLOAT_MAT2x4
#define GL_FLOAT_MAT2x4 0x8B66
#endif
#ifndef GL_FLOAT_MAT3x2
#define GL_FLOAT_MAT3x2 0x8B67
#endif
#ifndef GL_FLOAT_MAT3x4
#define GL_FLOAT_MAT3x4 0x8B68
#endif
#ifndef GL_FLOAT_MAT4x2
#define GL_FLOAT_MAT4x2 0x8B69
#endif
#ifndef GL_FLOAT_MAT4x3
#define GL_FLOAT_MAT4x3 0x8B6A
#endif
#ifndef GL_SRGB
#define GL_SRGB 0x8C40
#endif
#ifndef GL_SRGB8
#define GL_SRGB8 0x8C41
#endif
#ifndef GL_SRGB_ALPHA
#define GL_SRGB_ALPHA 0x8C42
#endif
#ifndef GL_SRGB8_ALPHA8
#define GL_SRGB8_ALPHA8 0x8C43
#endif
#ifndef GL_SLUMINANCE_ALPHA
#define GL_SLUMINANCE_ALPHA 0x8C44
#endif
#ifndef GL_SLUMINANCE8_ALPHA8
#define GL_SLUMINANCE8_ALPHA8 0x8C45
#endif
#ifndef GL_SLUMINANCE
#define GL_SLUMINANCE 0x8C46
#endif
#ifndef GL_SLUMINANCE8
#define GL_SLUMINANCE8 0x8C47
#endif
#ifndef GL_COMPRESSED_SRGB
#define GL_COMPRESSED_SRGB 0x8C48
#endif
#ifndef GL_COMPRESSED_SRGB_ALPHA
#define GL_COMPRESSED_SRGB_ALPHA 0x8C49
#endif
#ifndef GL_COMPRESSED_SLUMINANCE
#define GL_COMPRESSED_SLUMINANCE 0x8C4A
#endif
#ifndef GL_COMPRESSED_SLUMINANCE_ALPHA
#define GL_COMPRESSED_SLUMINANCE_ALPHA 0x8C4B
#endif
#ifndef GL_TEXTURE0_ARB
#define GL_TEXTURE0_ARB 0x84C0
#endif
#ifndef GL_TEXTURE1_ARB
#define GL_TEXTURE1_ARB 0x84C1
#endif
#ifndef GL_TEXTURE2_ARB
#define GL_TEXTURE2_ARB 0x84C2
#endif
#ifndef GL_TEXTURE3_ARB
#define GL_TEXTURE3_ARB 0x84C3
#endif
#ifndef GL_TEXTURE4_ARB
#define GL_TEXTURE4_ARB 0x84C4
#endif
#ifndef GL_TEXTURE5_ARB
#define GL_TEXTURE5_ARB 0x84C5
#endif
#ifndef GL_TEXTURE6_ARB
#define GL_TEXTURE6_ARB 0x84C6
#endif
#ifndef GL_TEXTURE7_ARB
#define GL_TEXTURE7_ARB 0x84C7
#endif
#ifndef GL_TEXTURE8_ARB
#define GL_TEXTURE8_ARB 0x84C8
#endif
#ifndef GL_TEXTURE9_ARB
#define GL_TEXTURE9_ARB 0x84C9
#endif
#ifndef GL_TEXTURE10_ARB
#define GL_TEXTURE10_ARB 0x84CA
#endif
#ifndef GL_TEXTURE11_ARB
#define GL_TEXTURE11_ARB 0x84CB
#endif
#ifndef GL_TEXTURE12_ARB
#define GL_TEXTURE12_ARB 0x84CC
#endif
#ifndef GL_TEXTURE13_ARB
#define GL_TEXTURE13_ARB 0x84CD
#endif
#ifndef GL_TEXTURE14_ARB
#define GL_TEXTURE14_ARB 0x84CE
#endif
#ifndef GL_TEXTURE15_ARB
#define GL_TEXTURE15_ARB 0x84CF
#endif
#ifndef GL_TEXTURE16_ARB
#define GL_TEXTURE16_ARB 0x84D0
#endif
#ifndef GL_TEXTURE17_ARB
#define GL_TEXTURE17_ARB 0x84D1
#endif
#ifndef GL_TEXTURE18_ARB
#define GL_TEXTURE18_ARB 0x84D2
#endif
#ifndef GL_TEXTURE19_ARB
#define GL_TEXTURE19_ARB 0x84D3
#endif
#ifndef GL_TEXTURE20_ARB
#define GL_TEXTURE20_ARB 0x84D4
#endif
#ifndef GL_TEXTURE21_ARB
#define GL_TEXTURE21_ARB 0x84D5
#endif
#ifndef GL_TEXTURE22_ARB
#define GL_TEXTURE22_ARB 0x84D6
#endif
#ifndef GL_TEXTURE23_ARB
#define GL_TEXTURE23_ARB 0x84D7
#endif
#ifndef GL_TEXTURE24_ARB
#define GL_TEXTURE24_ARB 0x84D8
#endif
#ifndef GL_TEXTURE25_ARB
#define GL_TEXTURE25_ARB 0x84D9
#endif
#ifndef GL_TEXTURE26_ARB
#define GL_TEXTURE26_ARB 0x84DA
#endif
#ifndef GL_TEXTURE27_ARB
#define GL_TEXTURE27_ARB 0x84DB
#endif
#ifndef GL_TEXTURE28_ARB
#define GL_TEXTURE28_ARB 0x84DC
#endif
#ifndef GL_TEXTURE29_ARB
#define GL_TEXTURE29_ARB 0x84DD
#endif
#ifndef GL_TEXTURE30_ARB
#define GL_TEXTURE30_ARB 0x84DE
#endif
#ifndef GL_TEXTURE31_ARB
#define GL_TEXTURE31_ARB 0x84DF
#endif
#ifndef GL_ACTIVE_TEXTURE_ARB
#define GL_ACTIVE_TEXTURE_ARB 0x84E0
#endif
#ifndef GL_CLIENT_ACTIVE_TEXTURE_ARB
#define GL_CLIENT_ACTIVE_TEXTURE_ARB 0x84E1
#endif
#ifndef GL_MAX_TEXTURE_UNITS_ARB
#define GL_MAX_TEXTURE_UNITS_ARB 0x84E2
#endif
#ifndef GL_TRANSPOSE_MODELVIEW_MATRIX_ARB
#define GL_TRANSPOSE_MODELVIEW_MATRIX_ARB 0x84E3
#endif
#ifndef GL_TRANSPOSE_PROJECTION_MATRIX_ARB
#define GL_TRANSPOSE_PROJECTION_MATRIX_ARB 0x84E4
#endif
#ifndef GL_TRANSPOSE_TEXTURE_MATRIX_ARB
#define GL_TRANSPOSE_TEXTURE_MATRIX_ARB 0x84E5
#endif
#ifndef GL_TRANSPOSE_COLOR_MATRIX_ARB
#define GL_TRANSPOSE_COLOR_MATRIX_ARB 0x84E6
#endif
#ifndef GL_MULTISAMPLE_ARB
#define GL_MULTISAMPLE_ARB 0x809D
#endif
#ifndef GL_SAMPLE_ALPHA_TO_COVERAGE_ARB
#define GL_SAMPLE_ALPHA_TO_COVERAGE_ARB 0x809E
#endif
#ifndef GL_SAMPLE_ALPHA_TO_ONE_ARB
#define GL_SAMPLE_ALPHA_TO_ONE_ARB 0x809F
#endif
#ifndef GL_SAMPLE_COVERAGE_ARB
#define GL_SAMPLE_COVERAGE_ARB 0x80A0
#endif
#ifndef GL_SAMPLE_BUFFERS_ARB
#define GL_SAMPLE_BUFFERS_ARB 0x80A8
#endif
#ifndef GL_SAMPLES_ARB
#define GL_SAMPLES_ARB 0x80A9
#endif
#ifndef GL_SAMPLE_COVERAGE_VALUE_ARB
#define GL_SAMPLE_COVERAGE_VALUE_ARB 0x80AA
#endif
#ifndef GL_SAMPLE_COVERAGE_INVERT_ARB
#define GL_SAMPLE_COVERAGE_INVERT_ARB 0x80AB
#endif
#ifndef GL_MULTISAMPLE_BIT_ARB
#define GL_MULTISAMPLE_BIT_ARB 0x20000000
#endif
#ifndef GL_NORMAL_MAP_ARB
#define GL_NORMAL_MAP_ARB 0x8511
#endif
#ifndef GL_REFLECTION_MAP_ARB
#define GL_REFLECTION_MAP_ARB 0x8512
#endif
#ifndef GL_TEXTURE_CUBE_MAP_ARB
#define GL_TEXTURE_CUBE_MAP_ARB 0x8513
#endif
#ifndef GL_TEXTURE_BINDING_CUBE_MAP_ARB
#define GL_TEXTURE_BINDING_CUBE_MAP_ARB 0x8514
#endif
#ifndef GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB 0x8515
#endif
#ifndef GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB 0x8516
#endif
#ifndef GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB 0x8517
#endif
#ifndef GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB 0x8518
#endif
#ifndef GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB 0x8519
#endif
#ifndef GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB 0x851A
#endif
#ifndef GL_PROXY_TEXTURE_CUBE_MAP_ARB
#define GL_PROXY_TEXTURE_CUBE_MAP_ARB 0x851B
#endif
#ifndef GL_MAX_CUBE_MAP_TEXTURE_SIZE_ARB
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE_ARB 0x851C
#endif
#ifndef GL_COMPRESSED_ALPHA_ARB
#define GL_COMPRESSED_ALPHA_ARB 0x84E9
#endif
#ifndef GL_COMPRESSED_LUMINANCE_ARB
#define GL_COMPRESSED_LUMINANCE_ARB 0x84EA
#endif
#ifndef GL_COMPRESSED_LUMINANCE_ALPHA_ARB
#define GL_COMPRESSED_LUMINANCE_ALPHA_ARB 0x84EB
#endif
#ifndef GL_COMPRESSED_INTENSITY_ARB
#define GL_COMPRESSED_INTENSITY_ARB 0x84EC
#endif
#ifndef GL_COMPRESSED_RGB_ARB
#define GL_COMPRESSED_RGB_ARB 0x84ED
#endif
#ifndef GL_COMPRESSED_RGBA_ARB
#define GL_COMPRESSED_RGBA_ARB 0x84EE
#endif
#ifndef GL_TEXTURE_COMPRESSION_HINT_ARB
#define GL_TEXTURE_COMPRESSION_HINT_ARB 0x84EF
#endif
#ifndef GL_TEXTURE_COMPRESSED_IMAGE_SIZE_ARB
#define GL_TEXTURE_COMPRESSED_IMAGE_SIZE_ARB 0x86A0
#endif
#ifndef GL_TEXTURE_COMPRESSED_ARB
#define GL_TEXTURE_COMPRESSED_ARB 0x86A1
#endif
#ifndef GL_NUM_COMPRESSED_TEXTURE_FORMATS_ARB
#define GL_NUM_COMPRESSED_TEXTURE_FORMATS_ARB 0x86A2
#endif
#ifndef GL_COMPRESSED_TEXTURE_FORMATS_ARB
#define GL_COMPRESSED_TEXTURE_FORMATS_ARB 0x86A3
#endif
#ifndef GL_CLAMP_TO_BORDER_ARB
#define GL_CLAMP_TO_BORDER_ARB 0x812D
#endif
#ifndef GL_POINT_SIZE_MIN_ARB
#define GL_POINT_SIZE_MIN_ARB 0x8126
#endif
#ifndef GL_POINT_SIZE_MAX_ARB
#define GL_POINT_SIZE_MAX_ARB 0x8127
#endif
#ifndef GL_POINT_FADE_THRESHOLD_SIZE_ARB
#define GL_POINT_FADE_THRESHOLD_SIZE_ARB 0x8128
#endif
#ifndef GL_POINT_DISTANCE_ATTENUATION_ARB
#define GL_POINT_DISTANCE_ATTENUATION_ARB 0x8129
#endif
#ifndef GL_MAX_VERTEX_UNITS_ARB
#define GL_MAX_VERTEX_UNITS_ARB 0x86A4
#endif
#ifndef GL_ACTIVE_VERTEX_UNITS_ARB
#define GL_ACTIVE_VERTEX_UNITS_ARB 0x86A5
#endif
#ifndef GL_WEIGHT_SUM_UNITY_ARB
#define GL_WEIGHT_SUM_UNITY_ARB 0x86A6
#endif
#ifndef GL_VERTEX_BLEND_ARB
#define GL_VERTEX_BLEND_ARB 0x86A7
#endif
#ifndef GL_CURRENT_WEIGHT_ARB
#define GL_CURRENT_WEIGHT_ARB 0x86A8
#endif
#ifndef GL_WEIGHT_ARRAY_TYPE_ARB
#define GL_WEIGHT_ARRAY_TYPE_ARB 0x86A9
#endif
#ifndef GL_WEIGHT_ARRAY_STRIDE_ARB
#define GL_WEIGHT_ARRAY_STRIDE_ARB 0x86AA
#endif
#ifndef GL_WEIGHT_ARRAY_SIZE_ARB
#define GL_WEIGHT_ARRAY_SIZE_ARB 0x86AB
#endif
#ifndef GL_WEIGHT_ARRAY_POINTER_ARB
#define GL_WEIGHT_ARRAY_POINTER_ARB 0x86AC
#endif
#ifndef GL_WEIGHT_ARRAY_ARB
#define GL_WEIGHT_ARRAY_ARB 0x86AD
#endif
#ifndef GL_MODELVIEW0_ARB
#define GL_MODELVIEW0_ARB 0x1700
#endif
#ifndef GL_MODELVIEW1_ARB
#define GL_MODELVIEW1_ARB 0x850A
#endif
#ifndef GL_MODELVIEW2_ARB
#define GL_MODELVIEW2_ARB 0x8722
#endif
#ifndef GL_MODELVIEW3_ARB
#define GL_MODELVIEW3_ARB 0x8723
#endif
#ifndef GL_MODELVIEW4_ARB
#define GL_MODELVIEW4_ARB 0x8724
#endif
#ifndef GL_MODELVIEW5_ARB
#define GL_MODELVIEW5_ARB 0x8725
#endif
#ifndef GL_MODELVIEW6_ARB
#define GL_MODELVIEW6_ARB 0x8726
#endif
#ifndef GL_MODELVIEW7_ARB
#define GL_MODELVIEW7_ARB 0x8727
#endif
#ifndef GL_MODELVIEW8_ARB
#define GL_MODELVIEW8_ARB 0x8728
#endif
#ifndef GL_MODELVIEW9_ARB
#define GL_MODELVIEW9_ARB 0x8729
#endif
#ifndef GL_MODELVIEW10_ARB
#define GL_MODELVIEW10_ARB 0x872A
#endif
#ifndef GL_MODELVIEW11_ARB
#define GL_MODELVIEW11_ARB 0x872B
#endif
#ifndef GL_MODELVIEW12_ARB
#define GL_MODELVIEW12_ARB 0x872C
#endif
#ifndef GL_MODELVIEW13_ARB
#define GL_MODELVIEW13_ARB 0x872D
#endif
#ifndef GL_MODELVIEW14_ARB
#define GL_MODELVIEW14_ARB 0x872E
#endif
#ifndef GL_MODELVIEW15_ARB
#define GL_MODELVIEW15_ARB 0x872F
#endif
#ifndef GL_MODELVIEW16_ARB
#define GL_MODELVIEW16_ARB 0x8730
#endif
#ifndef GL_MODELVIEW17_ARB
#define GL_MODELVIEW17_ARB 0x8731
#endif
#ifndef GL_MODELVIEW18_ARB
#define GL_MODELVIEW18_ARB 0x8732
#endif
#ifndef GL_MODELVIEW19_ARB
#define GL_MODELVIEW19_ARB 0x8733
#endif
#ifndef GL_MODELVIEW20_ARB
#define GL_MODELVIEW20_ARB 0x8734
#endif
#ifndef GL_MODELVIEW21_ARB
#define GL_MODELVIEW21_ARB 0x8735
#endif
#ifndef GL_MODELVIEW22_ARB
#define GL_MODELVIEW22_ARB 0x8736
#endif
#ifndef GL_MODELVIEW23_ARB
#define GL_MODELVIEW23_ARB 0x8737
#endif
#ifndef GL_MODELVIEW24_ARB
#define GL_MODELVIEW24_ARB 0x8738
#endif
#ifndef GL_MODELVIEW25_ARB
#define GL_MODELVIEW25_ARB 0x8739
#endif
#ifndef GL_MODELVIEW26_ARB
#define GL_MODELVIEW26_ARB 0x873A
#endif
#ifndef GL_MODELVIEW27_ARB
#define GL_MODELVIEW27_ARB 0x873B
#endif
#ifndef GL_MODELVIEW28_ARB
#define GL_MODELVIEW28_ARB 0x873C
#endif
#ifndef GL_MODELVIEW29_ARB
#define GL_MODELVIEW29_ARB 0x873D
#endif
#ifndef GL_MODELVIEW30_ARB
#define GL_MODELVIEW30_ARB 0x873E
#endif
#ifndef GL_MODELVIEW31_ARB
#define GL_MODELVIEW31_ARB 0x873F
#endif
#ifndef GL_MATRIX_PALETTE_ARB
#define GL_MATRIX_PALETTE_ARB 0x8840
#endif
#ifndef GL_MAX_MATRIX_PALETTE_STACK_DEPTH_ARB
#define GL_MAX_MATRIX_PALETTE_STACK_DEPTH_ARB 0x8841
#endif
#ifndef GL_MAX_PALETTE_MATRICES_ARB
#define GL_MAX_PALETTE_MATRICES_ARB 0x8842
#endif
#ifndef GL_CURRENT_PALETTE_MATRIX_ARB
#define GL_CURRENT_PALETTE_MATRIX_ARB 0x8843
#endif
#ifndef GL_MATRIX_INDEX_ARRAY_ARB
#define GL_MATRIX_INDEX_ARRAY_ARB 0x8844
#endif
#ifndef GL_CURRENT_MATRIX_INDEX_ARB
#define GL_CURRENT_MATRIX_INDEX_ARB 0x8845
#endif
#ifndef GL_MATRIX_INDEX_ARRAY_SIZE_ARB
#define GL_MATRIX_INDEX_ARRAY_SIZE_ARB 0x8846
#endif
#ifndef GL_MATRIX_INDEX_ARRAY_TYPE_ARB
#define GL_MATRIX_INDEX_ARRAY_TYPE_ARB 0x8847
#endif
#ifndef GL_MATRIX_INDEX_ARRAY_STRIDE_ARB
#define GL_MATRIX_INDEX_ARRAY_STRIDE_ARB 0x8848
#endif
#ifndef GL_MATRIX_INDEX_ARRAY_POINTER_ARB
#define GL_MATRIX_INDEX_ARRAY_POINTER_ARB 0x8849
#endif
#ifndef GL_COMBINE_ARB
#define GL_COMBINE_ARB 0x8570
#endif
#ifndef GL_COMBINE_RGB_ARB
#define GL_COMBINE_RGB_ARB 0x8571
#endif
#ifndef GL_COMBINE_ALPHA_ARB
#define GL_COMBINE_ALPHA_ARB 0x8572
#endif
#ifndef GL_SOURCE0_RGB_ARB
#define GL_SOURCE0_RGB_ARB 0x8580
#endif
#ifndef GL_SOURCE1_RGB_ARB
#define GL_SOURCE1_RGB_ARB 0x8581
#endif
#ifndef GL_SOURCE2_RGB_ARB
#define GL_SOURCE2_RGB_ARB 0x8582
#endif
#ifndef GL_SOURCE0_ALPHA_ARB
#define GL_SOURCE0_ALPHA_ARB 0x8588
#endif
#ifndef GL_SOURCE1_ALPHA_ARB
#define GL_SOURCE1_ALPHA_ARB 0x8589
#endif
#ifndef GL_SOURCE2_ALPHA_ARB
#define GL_SOURCE2_ALPHA_ARB 0x858A
#endif
#ifndef GL_OPERAND0_RGB_ARB
#define GL_OPERAND0_RGB_ARB 0x8590
#endif
#ifndef GL_OPERAND1_RGB_ARB
#define GL_OPERAND1_RGB_ARB 0x8591
#endif
#ifndef GL_OPERAND2_RGB_ARB
#define GL_OPERAND2_RGB_ARB 0x8592
#endif
#ifndef GL_OPERAND0_ALPHA_ARB
#define GL_OPERAND0_ALPHA_ARB 0x8598
#endif
#ifndef GL_OPERAND1_ALPHA_ARB
#define GL_OPERAND1_ALPHA_ARB 0x8599
#endif
#ifndef GL_OPERAND2_ALPHA_ARB
#define GL_OPERAND2_ALPHA_ARB 0x859A
#endif
#ifndef GL_RGB_SCALE_ARB
#define GL_RGB_SCALE_ARB 0x8573
#endif
#ifndef GL_ADD_SIGNED_ARB
#define GL_ADD_SIGNED_ARB 0x8574
#endif
#ifndef GL_INTERPOLATE_ARB
#define GL_INTERPOLATE_ARB 0x8575
#endif
#ifndef GL_SUBTRACT_ARB
#define GL_SUBTRACT_ARB 0x84E7
#endif
#ifndef GL_CONSTANT_ARB
#define GL_CONSTANT_ARB 0x8576
#endif
#ifndef GL_PRIMARY_COLOR_ARB
#define GL_PRIMARY_COLOR_ARB 0x8577
#endif
#ifndef GL_PREVIOUS_ARB
#define GL_PREVIOUS_ARB 0x8578
#endif
#ifndef GL_DOT3_RGB_ARB
#define GL_DOT3_RGB_ARB 0x86AE
#endif
#ifndef GL_DOT3_RGBA_ARB
#define GL_DOT3_RGBA_ARB 0x86AF
#endif
#ifndef GL_MIRRORED_REPEAT_ARB
#define GL_MIRRORED_REPEAT_ARB 0x8370
#endif
#ifndef GL_DEPTH_COMPONENT16_ARB
#define GL_DEPTH_COMPONENT16_ARB 0x81A5
#endif
#ifndef GL_DEPTH_COMPONENT24_ARB
#define GL_DEPTH_COMPONENT24_ARB 0x81A6
#endif
#ifndef GL_DEPTH_COMPONENT32_ARB
#define GL_DEPTH_COMPONENT32_ARB 0x81A7
#endif
#ifndef GL_TEXTURE_DEPTH_SIZE_ARB
#define GL_TEXTURE_DEPTH_SIZE_ARB 0x884A
#endif
#ifndef GL_DEPTH_TEXTURE_MODE_ARB
#define GL_DEPTH_TEXTURE_MODE_ARB 0x884B
#endif
#ifndef GL_TEXTURE_COMPARE_FAIL_VALUE_ARB
#define GL_TEXTURE_COMPARE_FAIL_VALUE_ARB 0x80BF
#endif
#ifndef GL_COLOR_SUM_ARB
#define GL_COLOR_SUM_ARB 0x8458
#endif
#ifndef GL_VERTEX_PROGRAM_ARB
#define GL_VERTEX_PROGRAM_ARB 0x8620
#endif
#ifndef GL_VERTEX_ATTRIB_ARRAY_ENABLED_ARB
#define GL_VERTEX_ATTRIB_ARRAY_ENABLED_ARB 0x8622
#endif
#ifndef GL_VERTEX_ATTRIB_ARRAY_SIZE_ARB
#define GL_VERTEX_ATTRIB_ARRAY_SIZE_ARB 0x8623
#endif
#ifndef GL_VERTEX_ATTRIB_ARRAY_STRIDE_ARB
#define GL_VERTEX_ATTRIB_ARRAY_STRIDE_ARB 0x8624
#endif
#ifndef GL_VERTEX_ATTRIB_ARRAY_TYPE_ARB
#define GL_VERTEX_ATTRIB_ARRAY_TYPE_ARB 0x8625
#endif
#ifndef GL_CURRENT_VERTEX_ATTRIB_ARB
#define GL_CURRENT_VERTEX_ATTRIB_ARB 0x8626
#endif
#ifndef GL_PROGRAM_LENGTH_ARB
#define GL_PROGRAM_LENGTH_ARB 0x8627
#endif
#ifndef GL_PROGRAM_STRING_ARB
#define GL_PROGRAM_STRING_ARB 0x8628
#endif
#ifndef GL_MAX_PROGRAM_MATRIX_STACK_DEPTH_ARB
#define GL_MAX_PROGRAM_MATRIX_STACK_DEPTH_ARB 0x862E
#endif
#ifndef GL_MAX_PROGRAM_MATRICES_ARB
#define GL_MAX_PROGRAM_MATRICES_ARB 0x862F
#endif
#ifndef GL_CURRENT_MATRIX_STACK_DEPTH_ARB
#define GL_CURRENT_MATRIX_STACK_DEPTH_ARB 0x8640
#endif
#ifndef GL_CURRENT_MATRIX_ARB
#define GL_CURRENT_MATRIX_ARB 0x8641
#endif
#ifndef GL_VERTEX_PROGRAM_POINT_SIZE_ARB
#define GL_VERTEX_PROGRAM_POINT_SIZE_ARB 0x8642
#endif
#ifndef GL_VERTEX_PROGRAM_TWO_SIDE_ARB
#define GL_VERTEX_PROGRAM_TWO_SIDE_ARB 0x8643
#endif
#ifndef GL_VERTEX_ATTRIB_ARRAY_POINTER_ARB
#define GL_VERTEX_ATTRIB_ARRAY_POINTER_ARB 0x8645
#endif
#ifndef GL_PROGRAM_ERROR_POSITION_ARB
#define GL_PROGRAM_ERROR_POSITION_ARB 0x864B
#endif
#ifndef GL_PROGRAM_BINDING_ARB
#define GL_PROGRAM_BINDING_ARB 0x8677
#endif
#ifndef GL_MAX_VERTEX_ATTRIBS_ARB
#define GL_MAX_VERTEX_ATTRIBS_ARB 0x8869
#endif
#ifndef GL_VERTEX_ATTRIB_ARRAY_NORMALIZED_ARB
#define GL_VERTEX_ATTRIB_ARRAY_NORMALIZED_ARB 0x886A
#endif
#ifndef GL_PROGRAM_ERROR_STRING_ARB
#define GL_PROGRAM_ERROR_STRING_ARB 0x8874
#endif
#ifndef GL_PROGRAM_FORMAT_ASCII_ARB
#define GL_PROGRAM_FORMAT_ASCII_ARB 0x8875
#endif
#ifndef GL_PROGRAM_FORMAT_ARB
#define GL_PROGRAM_FORMAT_ARB 0x8876
#endif
#ifndef GL_PROGRAM_INSTRUCTIONS_ARB
#define GL_PROGRAM_INSTRUCTIONS_ARB 0x88A0
#endif
#ifndef GL_MAX_PROGRAM_INSTRUCTIONS_ARB
#define GL_MAX_PROGRAM_INSTRUCTIONS_ARB 0x88A1
#endif
#ifndef GL_PROGRAM_NATIVE_INSTRUCTIONS_ARB
#define GL_PROGRAM_NATIVE_INSTRUCTIONS_ARB 0x88A2
#endif
#ifndef GL_MAX_PROGRAM_NATIVE_INSTRUCTIONS_ARB
#define GL_MAX_PROGRAM_NATIVE_INSTRUCTIONS_ARB 0x88A3
#endif
#ifndef GL_PROGRAM_TEMPORARIES_ARB
#define GL_PROGRAM_TEMPORARIES_ARB 0x88A4
#endif
#ifndef GL_MAX_PROGRAM_TEMPORARIES_ARB
#define GL_MAX_PROGRAM_TEMPORARIES_ARB 0x88A5
#endif
#ifndef GL_PROGRAM_NATIVE_TEMPORARIES_ARB
#define GL_PROGRAM_NATIVE_TEMPORARIES_ARB 0x88A6
#endif
#ifndef GL_MAX_PROGRAM_NATIVE_TEMPORARIES_ARB
#define GL_MAX_PROGRAM_NATIVE_TEMPORARIES_ARB 0x88A7
#endif
#ifndef GL_PROGRAM_PARAMETERS_ARB
#define GL_PROGRAM_PARAMETERS_ARB 0x88A8
#endif
#ifndef GL_MAX_PROGRAM_PARAMETERS_ARB
#define GL_MAX_PROGRAM_PARAMETERS_ARB 0x88A9
#endif
#ifndef GL_PROGRAM_NATIVE_PARAMETERS_ARB
#define GL_PROGRAM_NATIVE_PARAMETERS_ARB 0x88AA
#endif
#ifndef GL_MAX_PROGRAM_NATIVE_PARAMETERS_ARB
#define GL_MAX_PROGRAM_NATIVE_PARAMETERS_ARB 0x88AB
#endif
#ifndef GL_PROGRAM_ATTRIBS_ARB
#define GL_PROGRAM_ATTRIBS_ARB 0x88AC
#endif
#ifndef GL_MAX_PROGRAM_ATTRIBS_ARB
#define GL_MAX_PROGRAM_ATTRIBS_ARB 0x88AD
#endif
#ifndef GL_PROGRAM_NATIVE_ATTRIBS_ARB
#define GL_PROGRAM_NATIVE_ATTRIBS_ARB 0x88AE
#endif
#ifndef GL_MAX_PROGRAM_NATIVE_ATTRIBS_ARB
#define GL_MAX_PROGRAM_NATIVE_ATTRIBS_ARB 0x88AF
#endif
#ifndef GL_PROGRAM_ADDRESS_REGISTERS_ARB
#define GL_PROGRAM_ADDRESS_REGISTERS_ARB 0x88B0
#endif
#ifndef GL_MAX_PROGRAM_ADDRESS_REGISTERS_ARB
#define GL_MAX_PROGRAM_ADDRESS_REGISTERS_ARB 0x88B1
#endif
#ifndef GL_PROGRAM_NATIVE_ADDRESS_REGISTERS_ARB
#define GL_PROGRAM_NATIVE_ADDRESS_REGISTERS_ARB 0x88B2
#endif
#ifndef GL_MAX_PROGRAM_NATIVE_ADDRESS_REGISTERS_ARB
#define GL_MAX_PROGRAM_NATIVE_ADDRESS_REGISTERS_ARB 0x88B3
#endif
#ifndef GL_MAX_PROGRAM_LOCAL_PARAMETERS_ARB
#define GL_MAX_PROGRAM_LOCAL_PARAMETERS_ARB 0x88B4
#endif
#ifndef GL_MAX_PROGRAM_ENV_PARAMETERS_ARB
#define GL_MAX_PROGRAM_ENV_PARAMETERS_ARB 0x88B5
#endif
#ifndef GL_PROGRAM_UNDER_NATIVE_LIMITS_ARB
#define GL_PROGRAM_UNDER_NATIVE_LIMITS_ARB 0x88B6
#endif
#ifndef GL_TRANSPOSE_CURRENT_MATRIX_ARB
#define GL_TRANSPOSE_CURRENT_MATRIX_ARB 0x88B7
#endif
#ifndef GL_MATRIX0_ARB
#define GL_MATRIX0_ARB 0x88C0
#endif
#ifndef GL_MATRIX1_ARB
#define GL_MATRIX1_ARB 0x88C1
#endif
#ifndef GL_MATRIX2_ARB
#define GL_MATRIX2_ARB 0x88C2
#endif
#ifndef GL_MATRIX3_ARB
#define GL_MATRIX3_ARB 0x88C3
#endif
#ifndef GL_MATRIX4_ARB
#define GL_MATRIX4_ARB 0x88C4
#endif
#ifndef GL_MATRIX5_ARB
#define GL_MATRIX5_ARB 0x88C5
#endif
#ifndef GL_MATRIX6_ARB
#define GL_MATRIX6_ARB 0x88C6
#endif
#ifndef GL_MATRIX7_ARB
#define GL_MATRIX7_ARB 0x88C7
#endif
#ifndef GL_MATRIX8_ARB
#define GL_MATRIX8_ARB 0x88C8
#endif
#ifndef GL_MATRIX9_ARB
#define GL_MATRIX9_ARB 0x88C9
#endif
#ifndef GL_MATRIX10_ARB
#define GL_MATRIX10_ARB 0x88CA
#endif
#ifndef GL_MATRIX11_ARB
#define GL_MATRIX11_ARB 0x88CB
#endif
#ifndef GL_MATRIX12_ARB
#define GL_MATRIX12_ARB 0x88CC
#endif
#ifndef GL_MATRIX13_ARB
#define GL_MATRIX13_ARB 0x88CD
#endif
#ifndef GL_MATRIX14_ARB
#define GL_MATRIX14_ARB 0x88CE
#endif
#ifndef GL_MATRIX15_ARB
#define GL_MATRIX15_ARB 0x88CF
#endif
#ifndef GL_MATRIX16_ARB
#define GL_MATRIX16_ARB 0x88D0
#endif
#ifndef GL_MATRIX17_ARB
#define GL_MATRIX17_ARB 0x88D1
#endif
#ifndef GL_MATRIX18_ARB
#define GL_MATRIX18_ARB 0x88D2
#endif
#ifndef GL_MATRIX19_ARB
#define GL_MATRIX19_ARB 0x88D3
#endif
#ifndef GL_MATRIX20_ARB
#define GL_MATRIX20_ARB 0x88D4
#endif
#ifndef GL_MATRIX21_ARB
#define GL_MATRIX21_ARB 0x88D5
#endif
#ifndef GL_MATRIX22_ARB
#define GL_MATRIX22_ARB 0x88D6
#endif
#ifndef GL_MATRIX23_ARB
#define GL_MATRIX23_ARB 0x88D7
#endif
#ifndef GL_MATRIX24_ARB
#define GL_MATRIX24_ARB 0x88D8
#endif
#ifndef GL_MATRIX25_ARB
#define GL_MATRIX25_ARB 0x88D9
#endif
#ifndef GL_MATRIX26_ARB
#define GL_MATRIX26_ARB 0x88DA
#endif
#ifndef GL_MATRIX27_ARB
#define GL_MATRIX27_ARB 0x88DB
#endif
#ifndef GL_MATRIX28_ARB
#define GL_MATRIX28_ARB 0x88DC
#endif
#ifndef GL_MATRIX29_ARB
#define GL_MATRIX29_ARB 0x88DD
#endif
#ifndef GL_MATRIX30_ARB
#define GL_MATRIX30_ARB 0x88DE
#endif
#ifndef GL_MATRIX31_ARB
#define GL_MATRIX31_ARB 0x88DF
#endif
#ifndef GL_FRAGMENT_PROGRAM_ARB
#define GL_FRAGMENT_PROGRAM_ARB 0x8804
#endif
#ifndef GL_PROGRAM_ALU_INSTRUCTIONS_ARB
#define GL_PROGRAM_ALU_INSTRUCTIONS_ARB 0x8805
#endif
#ifndef GL_PROGRAM_TEX_INSTRUCTIONS_ARB
#define GL_PROGRAM_TEX_INSTRUCTIONS_ARB 0x8806
#endif
#ifndef GL_PROGRAM_TEX_INDIRECTIONS_ARB
#define GL_PROGRAM_TEX_INDIRECTIONS_ARB 0x8807
#endif
#ifndef GL_PROGRAM_NATIVE_ALU_INSTRUCTIONS_ARB
#define GL_PROGRAM_NATIVE_ALU_INSTRUCTIONS_ARB 0x8808
#endif
#ifndef GL_PROGRAM_NATIVE_TEX_INSTRUCTIONS_ARB
#define GL_PROGRAM_NATIVE_TEX_INSTRUCTIONS_ARB 0x8809
#endif
#ifndef GL_PROGRAM_NATIVE_TEX_INDIRECTIONS_ARB
#define GL_PROGRAM_NATIVE_TEX_INDIRECTIONS_ARB 0x880A
#endif
#ifndef GL_MAX_PROGRAM_ALU_INSTRUCTIONS_ARB
#define GL_MAX_PROGRAM_ALU_INSTRUCTIONS_ARB 0x880B
#endif
#ifndef GL_MAX_PROGRAM_TEX_INSTRUCTIONS_ARB
#define GL_MAX_PROGRAM_TEX_INSTRUCTIONS_ARB 0x880C
#endif
#ifndef GL_MAX_PROGRAM_TEX_INDIRECTIONS_ARB
#define GL_MAX_PROGRAM_TEX_INDIRECTIONS_ARB 0x880D
#endif
#ifndef GL_MAX_PROGRAM_NATIVE_ALU_INSTRUCTIONS_ARB
#define GL_MAX_PROGRAM_NATIVE_ALU_INSTRUCTIONS_ARB 0x880E
#endif
#ifndef GL_MAX_PROGRAM_NATIVE_TEX_INSTRUCTIONS_ARB
#define GL_MAX_PROGRAM_NATIVE_TEX_INSTRUCTIONS_ARB 0x880F
#endif
#ifndef GL_MAX_PROGRAM_NATIVE_TEX_INDIRECTIONS_ARB
#define GL_MAX_PROGRAM_NATIVE_TEX_INDIRECTIONS_ARB 0x8810
#endif
#ifndef GL_MAX_TEXTURE_COORDS_ARB
#define GL_MAX_TEXTURE_COORDS_ARB 0x8871
#endif
#ifndef GL_MAX_TEXTURE_IMAGE_UNITS_ARB
#define GL_MAX_TEXTURE_IMAGE_UNITS_ARB 0x8872
#endif
#ifndef GL_BUFFER_SIZE_ARB
#define GL_BUFFER_SIZE_ARB 0x8764
#endif
#ifndef GL_BUFFER_USAGE_ARB
#define GL_BUFFER_USAGE_ARB 0x8765
#endif
#ifndef GL_ARRAY_BUFFER_ARB
#define GL_ARRAY_BUFFER_ARB 0x8892
#endif
#ifndef GL_ELEMENT_ARRAY_BUFFER_ARB
#define GL_ELEMENT_ARRAY_BUFFER_ARB 0x8893
#endif
#ifndef GL_ARRAY_BUFFER_BINDING_ARB
#define GL_ARRAY_BUFFER_BINDING_ARB 0x8894
#endif
#ifndef GL_ELEMENT_ARRAY_BUFFER_BINDING_ARB
#define GL_ELEMENT_ARRAY_BUFFER_BINDING_ARB 0x8895
#endif
#ifndef GL_VERTEX_ARRAY_BUFFER_BINDING_ARB
#define GL_VERTEX_ARRAY_BUFFER_BINDING_ARB 0x8896
#endif
#ifndef GL_NORMAL_ARRAY_BUFFER_BINDING_ARB
#define GL_NORMAL_ARRAY_BUFFER_BINDING_ARB 0x8897
#endif
#ifndef GL_COLOR_ARRAY_BUFFER_BINDING_ARB
#define GL_COLOR_ARRAY_BUFFER_BINDING_ARB 0x8898
#endif
#ifndef GL_INDEX_ARRAY_BUFFER_BINDING_ARB
#define GL_INDEX_ARRAY_BUFFER_BINDING_ARB 0x8899
#endif
#ifndef GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING_ARB
#define GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING_ARB 0x889A
#endif
#ifndef GL_EDGE_FLAG_ARRAY_BUFFER_BINDING_ARB
#define GL_EDGE_FLAG_ARRAY_BUFFER_BINDING_ARB 0x889B
#endif
#ifndef GL_SECONDARY_COLOR_ARRAY_BUFFER_BINDING_ARB
#define GL_SECONDARY_COLOR_ARRAY_BUFFER_BINDING_ARB 0x889C
#endif
#ifndef GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING_ARB
#define GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING_ARB 0x889D
#endif
#ifndef GL_WEIGHT_ARRAY_BUFFER_BINDING_ARB
#define GL_WEIGHT_ARRAY_BUFFER_BINDING_ARB 0x889E
#endif
#ifndef GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING_ARB
#define GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING_ARB 0x889F
#endif
#ifndef GL_READ_ONLY_ARB
#define GL_READ_ONLY_ARB 0x88B8
#endif
#ifndef GL_WRITE_ONLY_ARB
#define GL_WRITE_ONLY_ARB 0x88B9
#endif
#ifndef GL_READ_WRITE_ARB
#define GL_READ_WRITE_ARB 0x88BA
#endif
#ifndef GL_BUFFER_ACCESS_ARB
#define GL_BUFFER_ACCESS_ARB 0x88BB
#endif
#ifndef GL_BUFFER_MAPPED_ARB
#define GL_BUFFER_MAPPED_ARB 0x88BC
#endif
#ifndef GL_BUFFER_MAP_POINTER_ARB
#define GL_BUFFER_MAP_POINTER_ARB 0x88BD
#endif
#ifndef GL_STREAM_DRAW_ARB
#define GL_STREAM_DRAW_ARB 0x88E0
#endif
#ifndef GL_STREAM_READ_ARB
#define GL_STREAM_READ_ARB 0x88E1
#endif
#ifndef GL_STREAM_COPY_ARB
#define GL_STREAM_COPY_ARB 0x88E2
#endif
#ifndef GL_STATIC_DRAW_ARB
#define GL_STATIC_DRAW_ARB 0x88E4
#endif
#ifndef GL_STATIC_READ_ARB
#define GL_STATIC_READ_ARB 0x88E5
#endif
#ifndef GL_STATIC_COPY_ARB
#define GL_STATIC_COPY_ARB 0x88E6
#endif
#ifndef GL_DYNAMIC_DRAW_ARB
#define GL_DYNAMIC_DRAW_ARB 0x88E8
#endif
#ifndef GL_DYNAMIC_READ_ARB
#define GL_DYNAMIC_READ_ARB 0x88E9
#endif
#ifndef GL_DYNAMIC_COPY_ARB
#define GL_DYNAMIC_COPY_ARB 0x88EA
#endif
#ifndef GL_QUERY_COUNTER_BITS_ARB
#define GL_QUERY_COUNTER_BITS_ARB 0x8864
#endif
#ifndef GL_CURRENT_QUERY_ARB
#define GL_CURRENT_QUERY_ARB 0x8865
#endif
#ifndef GL_QUERY_RESULT_ARB
#define GL_QUERY_RESULT_ARB 0x8866
#endif
#ifndef GL_QUERY_RESULT_AVAILABLE_ARB
#define GL_QUERY_RESULT_AVAILABLE_ARB 0x8867
#endif
#ifndef GL_SAMPLES_PASSED_ARB
#define GL_SAMPLES_PASSED_ARB 0x8914
#endif
#ifndef GL_PROGRAM_OBJECT_ARB
#define GL_PROGRAM_OBJECT_ARB 0x8B40
#endif
#ifndef GL_SHADER_OBJECT_ARB
#define GL_SHADER_OBJECT_ARB 0x8B48
#endif
#ifndef GL_OBJECT_TYPE_ARB
#define GL_OBJECT_TYPE_ARB 0x8B4E
#endif
#ifndef GL_OBJECT_SUBTYPE_ARB
#define GL_OBJECT_SUBTYPE_ARB 0x8B4F
#endif
#ifndef GL_FLOAT_VEC2_ARB
#define GL_FLOAT_VEC2_ARB 0x8B50
#endif
#ifndef GL_FLOAT_VEC3_ARB
#define GL_FLOAT_VEC3_ARB 0x8B51
#endif
#ifndef GL_FLOAT_VEC4_ARB
#define GL_FLOAT_VEC4_ARB 0x8B52
#endif
#ifndef GL_INT_VEC2_ARB
#define GL_INT_VEC2_ARB 0x8B53
#endif
#ifndef GL_INT_VEC3_ARB
#define GL_INT_VEC3_ARB 0x8B54
#endif
#ifndef GL_INT_VEC4_ARB
#define GL_INT_VEC4_ARB 0x8B55
#endif
#ifndef GL_BOOL_ARB
#define GL_BOOL_ARB 0x8B56
#endif
#ifndef GL_BOOL_VEC2_ARB
#define GL_BOOL_VEC2_ARB 0x8B57
#endif
#ifndef GL_BOOL_VEC3_ARB
#define GL_BOOL_VEC3_ARB 0x8B58
#endif
#ifndef GL_BOOL_VEC4_ARB
#define GL_BOOL_VEC4_ARB 0x8B59
#endif
#ifndef GL_FLOAT_MAT2_ARB
#define GL_FLOAT_MAT2_ARB 0x8B5A
#endif
#ifndef GL_FLOAT_MAT3_ARB
#define GL_FLOAT_MAT3_ARB 0x8B5B
#endif
#ifndef GL_FLOAT_MAT4_ARB
#define GL_FLOAT_MAT4_ARB 0x8B5C
#endif
#ifndef GL_SAMPLER_1D_ARB
#define GL_SAMPLER_1D_ARB 0x8B5D
#endif
#ifndef GL_SAMPLER_2D_ARB
#define GL_SAMPLER_2D_ARB 0x8B5E
#endif
#ifndef GL_SAMPLER_3D_ARB
#define GL_SAMPLER_3D_ARB 0x8B5F
#endif
#ifndef GL_SAMPLER_CUBE_ARB
#define GL_SAMPLER_CUBE_ARB 0x8B60
#endif
#ifndef GL_SAMPLER_1D_SHADOW_ARB
#define GL_SAMPLER_1D_SHADOW_ARB 0x8B61
#endif
#ifndef GL_SAMPLER_2D_SHADOW_ARB
#define GL_SAMPLER_2D_SHADOW_ARB 0x8B62
#endif
#ifndef GL_SAMPLER_2D_RECT_ARB
#define GL_SAMPLER_2D_RECT_ARB 0x8B63
#endif
#ifndef GL_SAMPLER_2D_RECT_SHADOW_ARB
#define GL_SAMPLER_2D_RECT_SHADOW_ARB 0x8B64
#endif
#ifndef GL_OBJECT_DELETE_STATUS_ARB
#define GL_OBJECT_DELETE_STATUS_ARB 0x8B80
#endif
#ifndef GL_OBJECT_COMPILE_STATUS_ARB
#define GL_OBJECT_COMPILE_STATUS_ARB 0x8B81
#endif
#ifndef GL_OBJECT_LINK_STATUS_ARB
#define GL_OBJECT_LINK_STATUS_ARB 0x8B82
#endif
#ifndef GL_OBJECT_VALIDATE_STATUS_ARB
#define GL_OBJECT_VALIDATE_STATUS_ARB 0x8B83
#endif
#ifndef GL_OBJECT_INFO_LOG_LENGTH_ARB
#define GL_OBJECT_INFO_LOG_LENGTH_ARB 0x8B84
#endif
#ifndef GL_OBJECT_ATTACHED_OBJECTS_ARB
#define GL_OBJECT_ATTACHED_OBJECTS_ARB 0x8B85
#endif
#ifndef GL_OBJECT_ACTIVE_UNIFORMS_ARB
#define GL_OBJECT_ACTIVE_UNIFORMS_ARB 0x8B86
#endif
#ifndef GL_OBJECT_ACTIVE_UNIFORM_MAX_LENGTH_ARB
#define GL_OBJECT_ACTIVE_UNIFORM_MAX_LENGTH_ARB 0x8B87
#endif
#ifndef GL_OBJECT_SHADER_SOURCE_LENGTH_ARB
#define GL_OBJECT_SHADER_SOURCE_LENGTH_ARB 0x8B88
#endif
#ifndef GL_VERTEX_SHADER_ARB
#define GL_VERTEX_SHADER_ARB 0x8B31
#endif
#ifndef GL_MAX_VERTEX_UNIFORM_COMPONENTS_ARB
#define GL_MAX_VERTEX_UNIFORM_COMPONENTS_ARB 0x8B4A
#endif
#ifndef GL_MAX_VARYING_FLOATS_ARB
#define GL_MAX_VARYING_FLOATS_ARB 0x8B4B
#endif
#ifndef GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS_ARB
#define GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS_ARB 0x8B4C
#endif
#ifndef GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS_ARB
#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS_ARB 0x8B4D
#endif
#ifndef GL_OBJECT_ACTIVE_ATTRIBUTES_ARB
#define GL_OBJECT_ACTIVE_ATTRIBUTES_ARB 0x8B89
#endif
#ifndef GL_OBJECT_ACTIVE_ATTRIBUTE_MAX_LENGTH_ARB
#define GL_OBJECT_ACTIVE_ATTRIBUTE_MAX_LENGTH_ARB 0x8B8A
#endif
#ifndef GL_FRAGMENT_SHADER_ARB
#define GL_FRAGMENT_SHADER_ARB 0x8B30
#endif
#ifndef GL_MAX_FRAGMENT_UNIFORM_COMPONENTS_ARB
#define GL_MAX_FRAGMENT_UNIFORM_COMPONENTS_ARB 0x8B49
#endif
#ifndef GL_FRAGMENT_SHADER_DERIVATIVE_HINT_ARB
#define GL_FRAGMENT_SHADER_DERIVATIVE_HINT_ARB 0x8B8B
#endif
#ifndef GL_SHADING_LANGUAGE_VERSION_ARB
#define GL_SHADING_LANGUAGE_VERSION_ARB 0x8B8C
#endif
#ifndef GL_POINT_SPRITE_ARB
#define GL_POINT_SPRITE_ARB 0x8861
#endif
#ifndef GL_COORD_REPLACE_ARB
#define GL_COORD_REPLACE_ARB 0x8862
#endif
#ifndef GL_MAX_DRAW_BUFFERS_ARB
#define GL_MAX_DRAW_BUFFERS_ARB 0x8824
#endif
#ifndef GL_DRAW_BUFFER0_ARB
#define GL_DRAW_BUFFER0_ARB 0x8825
#endif
#ifndef GL_DRAW_BUFFER1_ARB
#define GL_DRAW_BUFFER1_ARB 0x8826
#endif
#ifndef GL_DRAW_BUFFER2_ARB
#define GL_DRAW_BUFFER2_ARB 0x8827
#endif
#ifndef GL_DRAW_BUFFER3_ARB
#define GL_DRAW_BUFFER3_ARB 0x8828
#endif
#ifndef GL_DRAW_BUFFER4_ARB
#define GL_DRAW_BUFFER4_ARB 0x8829
#endif
#ifndef GL_DRAW_BUFFER5_ARB
#define GL_DRAW_BUFFER5_ARB 0x882A
#endif
#ifndef GL_DRAW_BUFFER6_ARB
#define GL_DRAW_BUFFER6_ARB 0x882B
#endif
#ifndef GL_DRAW_BUFFER7_ARB
#define GL_DRAW_BUFFER7_ARB 0x882C
#endif
#ifndef GL_DRAW_BUFFER8_ARB
#define GL_DRAW_BUFFER8_ARB 0x882D
#endif
#ifndef GL_DRAW_BUFFER9_ARB
#define GL_DRAW_BUFFER9_ARB 0x882E
#endif
#ifndef GL_DRAW_BUFFER10_ARB
#define GL_DRAW_BUFFER10_ARB 0x882F
#endif
#ifndef GL_DRAW_BUFFER11_ARB
#define GL_DRAW_BUFFER11_ARB 0x8830
#endif
#ifndef GL_DRAW_BUFFER12_ARB
#define GL_DRAW_BUFFER12_ARB 0x8831
#endif
#ifndef GL_DRAW_BUFFER13_ARB
#define GL_DRAW_BUFFER13_ARB 0x8832
#endif
#ifndef GL_DRAW_BUFFER14_ARB
#define GL_DRAW_BUFFER14_ARB 0x8833
#endif
#ifndef GL_DRAW_BUFFER15_ARB
#define GL_DRAW_BUFFER15_ARB 0x8834
#endif
#ifndef GL_TEXTURE_RECTANGLE_ARB
#define GL_TEXTURE_RECTANGLE_ARB 0x84F5
#endif
#ifndef GL_TEXTURE_BINDING_RECTANGLE_ARB
#define GL_TEXTURE_BINDING_RECTANGLE_ARB 0x84F6
#endif
#ifndef GL_PROXY_TEXTURE_RECTANGLE_ARB
#define GL_PROXY_TEXTURE_RECTANGLE_ARB 0x84F7
#endif
#ifndef GL_MAX_RECTANGLE_TEXTURE_SIZE_ARB
#define GL_MAX_RECTANGLE_TEXTURE_SIZE_ARB 0x84F8
#endif
#ifndef GL_RGBA_FLOAT_MODE_ARB
#define GL_RGBA_FLOAT_MODE_ARB 0x8820
#endif
#ifndef GL_CLAMP_VERTEX_COLOR_ARB
#define GL_CLAMP_VERTEX_COLOR_ARB 0x891A
#endif
#ifndef GL_CLAMP_FRAGMENT_COLOR_ARB
#define GL_CLAMP_FRAGMENT_COLOR_ARB 0x891B
#endif
#ifndef GL_CLAMP_READ_COLOR_ARB
#define GL_CLAMP_READ_COLOR_ARB 0x891C
#endif
#ifndef GL_FIXED_ONLY_ARB
#define GL_FIXED_ONLY_ARB 0x891D
#endif
#ifndef GL_HALF_FLOAT_ARB
#define GL_HALF_FLOAT_ARB 0x140B
#endif
#ifndef GL_TEXTURE_RED_TYPE_ARB
#define GL_TEXTURE_RED_TYPE_ARB 0x8C10
#endif
#ifndef GL_TEXTURE_GREEN_TYPE_ARB
#define GL_TEXTURE_GREEN_TYPE_ARB 0x8C11
#endif
#ifndef GL_TEXTURE_BLUE_TYPE_ARB
#define GL_TEXTURE_BLUE_TYPE_ARB 0x8C12
#endif
#ifndef GL_TEXTURE_ALPHA_TYPE_ARB
#define GL_TEXTURE_ALPHA_TYPE_ARB 0x8C13
#endif
#ifndef GL_TEXTURE_LUMINANCE_TYPE_ARB
#define GL_TEXTURE_LUMINANCE_TYPE_ARB 0x8C14
#endif
#ifndef GL_TEXTURE_INTENSITY_TYPE_ARB
#define GL_TEXTURE_INTENSITY_TYPE_ARB 0x8C15
#endif
#ifndef GL_TEXTURE_DEPTH_TYPE_ARB
#define GL_TEXTURE_DEPTH_TYPE_ARB 0x8C16
#endif
#ifndef GL_UNSIGNED_NORMALIZED_ARB
#define GL_UNSIGNED_NORMALIZED_ARB 0x8C17
#endif
#ifndef GL_RGBA32F_ARB
#define GL_RGBA32F_ARB 0x8814
#endif
#ifndef GL_RGB32F_ARB
#define GL_RGB32F_ARB 0x8815
#endif
#ifndef GL_ALPHA32F_ARB
#define GL_ALPHA32F_ARB 0x8816
#endif
#ifndef GL_INTENSITY32F_ARB
#define GL_INTENSITY32F_ARB 0x8817
#endif
#ifndef GL_LUMINANCE32F_ARB
#define GL_LUMINANCE32F_ARB 0x8818
#endif
#ifndef GL_LUMINANCE_ALPHA32F_ARB
#define GL_LUMINANCE_ALPHA32F_ARB 0x8819
#endif
#ifndef GL_RGBA16F_ARB
#define GL_RGBA16F_ARB 0x881A
#endif
#ifndef GL_RGB16F_ARB
#define GL_RGB16F_ARB 0x881B
#endif
#ifndef GL_ALPHA16F_ARB
#define GL_ALPHA16F_ARB 0x881C
#endif
#ifndef GL_INTENSITY16F_ARB
#define GL_INTENSITY16F_ARB 0x881D
#endif
#ifndef GL_LUMINANCE16F_ARB
#define GL_LUMINANCE16F_ARB 0x881E
#endif
#ifndef GL_LUMINANCE_ALPHA16F_ARB
#define GL_LUMINANCE_ALPHA16F_ARB 0x881F
#endif
#ifndef GL_PIXEL_PACK_BUFFER_ARB
#define GL_PIXEL_PACK_BUFFER_ARB 0x88EB
#endif
#ifndef GL_PIXEL_UNPACK_BUFFER_ARB
#define GL_PIXEL_UNPACK_BUFFER_ARB 0x88EC
#endif
#ifndef GL_PIXEL_PACK_BUFFER_BINDING_ARB
#define GL_PIXEL_PACK_BUFFER_BINDING_ARB 0x88ED
#endif
#ifndef GL_PIXEL_UNPACK_BUFFER_BINDING_ARB
#define GL_PIXEL_UNPACK_BUFFER_BINDING_ARB 0x88EF
#endif
#ifndef GL_ABGR_EXT
#define GL_ABGR_EXT 0x8000
#endif
#ifndef GL_CONSTANT_COLOR_EXT
#define GL_CONSTANT_COLOR_EXT 0x8001
#endif
#ifndef GL_ONE_MINUS_CONSTANT_COLOR_EXT
#define GL_ONE_MINUS_CONSTANT_COLOR_EXT 0x8002
#endif
#ifndef GL_CONSTANT_ALPHA_EXT
#define GL_CONSTANT_ALPHA_EXT 0x8003
#endif
#ifndef GL_ONE_MINUS_CONSTANT_ALPHA_EXT
#define GL_ONE_MINUS_CONSTANT_ALPHA_EXT 0x8004
#endif
#ifndef GL_BLEND_COLOR_EXT
#define GL_BLEND_COLOR_EXT 0x8005
#endif
#ifndef GL_POLYGON_OFFSET_EXT
#define GL_POLYGON_OFFSET_EXT 0x8037
#endif
#ifndef GL_POLYGON_OFFSET_FACTOR_EXT
#define GL_POLYGON_OFFSET_FACTOR_EXT 0x8038
#endif
#ifndef GL_POLYGON_OFFSET_BIAS_EXT
#define GL_POLYGON_OFFSET_BIAS_EXT 0x8039
#endif
#ifndef GL_ALPHA4_EXT
#define GL_ALPHA4_EXT 0x803B
#endif
#ifndef GL_ALPHA8_EXT
#define GL_ALPHA8_EXT 0x803C
#endif
#ifndef GL_ALPHA12_EXT
#define GL_ALPHA12_EXT 0x803D
#endif
#ifndef GL_ALPHA16_EXT
#define GL_ALPHA16_EXT 0x803E
#endif
#ifndef GL_LUMINANCE4_EXT
#define GL_LUMINANCE4_EXT 0x803F
#endif
#ifndef GL_LUMINANCE8_EXT
#define GL_LUMINANCE8_EXT 0x8040
#endif
#ifndef GL_LUMINANCE12_EXT
#define GL_LUMINANCE12_EXT 0x8041
#endif
#ifndef GL_LUMINANCE16_EXT
#define GL_LUMINANCE16_EXT 0x8042
#endif
#ifndef GL_LUMINANCE4_ALPHA4_EXT
#define GL_LUMINANCE4_ALPHA4_EXT 0x8043
#endif
#ifndef GL_LUMINANCE6_ALPHA2_EXT
#define GL_LUMINANCE6_ALPHA2_EXT 0x8044
#endif
#ifndef GL_LUMINANCE8_ALPHA8_EXT
#define GL_LUMINANCE8_ALPHA8_EXT 0x8045
#endif
#ifndef GL_LUMINANCE12_ALPHA4_EXT
#define GL_LUMINANCE12_ALPHA4_EXT 0x8046
#endif
#ifndef GL_LUMINANCE12_ALPHA12_EXT
#define GL_LUMINANCE12_ALPHA12_EXT 0x8047
#endif
#ifndef GL_LUMINANCE16_ALPHA16_EXT
#define GL_LUMINANCE16_ALPHA16_EXT 0x8048
#endif
#ifndef GL_INTENSITY_EXT
#define GL_INTENSITY_EXT 0x8049
#endif
#ifndef GL_INTENSITY4_EXT
#define GL_INTENSITY4_EXT 0x804A
#endif
#ifndef GL_INTENSITY8_EXT
#define GL_INTENSITY8_EXT 0x804B
#endif
#ifndef GL_INTENSITY12_EXT
#define GL_INTENSITY12_EXT 0x804C
#endif
#ifndef GL_INTENSITY16_EXT
#define GL_INTENSITY16_EXT 0x804D
#endif
#ifndef GL_RGB2_EXT
#define GL_RGB2_EXT 0x804E
#endif
#ifndef GL_RGB4_EXT
#define GL_RGB4_EXT 0x804F
#endif
#ifndef GL_RGB5_EXT
#define GL_RGB5_EXT 0x8050
#endif
#ifndef GL_RGB8_EXT
#define GL_RGB8_EXT 0x8051
#endif
#ifndef GL_RGB10_EXT
#define GL_RGB10_EXT 0x8052
#endif
#ifndef GL_RGB12_EXT
#define GL_RGB12_EXT 0x8053
#endif
#ifndef GL_RGB16_EXT
#define GL_RGB16_EXT 0x8054
#endif
#ifndef GL_RGBA2_EXT
#define GL_RGBA2_EXT 0x8055
#endif
#ifndef GL_RGBA4_EXT
#define GL_RGBA4_EXT 0x8056
#endif
#ifndef GL_RGB5_A1_EXT
#define GL_RGB5_A1_EXT 0x8057
#endif
#ifndef GL_RGBA8_EXT
#define GL_RGBA8_EXT 0x8058
#endif
#ifndef GL_RGB10_A2_EXT
#define GL_RGB10_A2_EXT 0x8059
#endif
#ifndef GL_RGBA12_EXT
#define GL_RGBA12_EXT 0x805A
#endif
#ifndef GL_RGBA16_EXT
#define GL_RGBA16_EXT 0x805B
#endif
#ifndef GL_TEXTURE_RED_SIZE_EXT
#define GL_TEXTURE_RED_SIZE_EXT 0x805C
#endif
#ifndef GL_TEXTURE_GREEN_SIZE_EXT
#define GL_TEXTURE_GREEN_SIZE_EXT 0x805D
#endif
#ifndef GL_TEXTURE_BLUE_SIZE_EXT
#define GL_TEXTURE_BLUE_SIZE_EXT 0x805E
#endif
#ifndef GL_TEXTURE_ALPHA_SIZE_EXT
#define GL_TEXTURE_ALPHA_SIZE_EXT 0x805F
#endif
#ifndef GL_TEXTURE_LUMINANCE_SIZE_EXT
#define GL_TEXTURE_LUMINANCE_SIZE_EXT 0x8060
#endif
#ifndef GL_TEXTURE_INTENSITY_SIZE_EXT
#define GL_TEXTURE_INTENSITY_SIZE_EXT 0x8061
#endif
#ifndef GL_REPLACE_EXT
#define GL_REPLACE_EXT 0x8062
#endif
#ifndef GL_PROXY_TEXTURE_1D_EXT
#define GL_PROXY_TEXTURE_1D_EXT 0x8063
#endif
#ifndef GL_PROXY_TEXTURE_2D_EXT
#define GL_PROXY_TEXTURE_2D_EXT 0x8064
#endif
#ifndef GL_TEXTURE_TOO_LARGE_EXT
#define GL_TEXTURE_TOO_LARGE_EXT 0x8065
#endif
#ifndef GL_PACK_SKIP_IMAGES_EXT
#define GL_PACK_SKIP_IMAGES_EXT 0x806B
#endif
#ifndef GL_PACK_IMAGE_HEIGHT_EXT
#define GL_PACK_IMAGE_HEIGHT_EXT 0x806C
#endif
#ifndef GL_UNPACK_SKIP_IMAGES_EXT
#define GL_UNPACK_SKIP_IMAGES_EXT 0x806D
#endif
#ifndef GL_UNPACK_IMAGE_HEIGHT_EXT
#define GL_UNPACK_IMAGE_HEIGHT_EXT 0x806E
#endif
#ifndef GL_TEXTURE_3D_EXT
#define GL_TEXTURE_3D_EXT 0x806F
#endif
#ifndef GL_PROXY_TEXTURE_3D_EXT
#define GL_PROXY_TEXTURE_3D_EXT 0x8070
#endif
#ifndef GL_TEXTURE_DEPTH_EXT
#define GL_TEXTURE_DEPTH_EXT 0x8071
#endif
#ifndef GL_TEXTURE_WRAP_R_EXT
#define GL_TEXTURE_WRAP_R_EXT 0x8072
#endif
#ifndef GL_MAX_3D_TEXTURE_SIZE_EXT
#define GL_MAX_3D_TEXTURE_SIZE_EXT 0x8073
#endif
#ifndef GL_FILTER4_SGIS
#define GL_FILTER4_SGIS 0x8146
#endif
#ifndef GL_TEXTURE_FILTER4_SIZE_SGIS
#define GL_TEXTURE_FILTER4_SIZE_SGIS 0x8147
#endif
#ifndef GL_HISTOGRAM_EXT
#define GL_HISTOGRAM_EXT 0x8024
#endif
#ifndef GL_PROXY_HISTOGRAM_EXT
#define GL_PROXY_HISTOGRAM_EXT 0x8025
#endif
#ifndef GL_HISTOGRAM_WIDTH_EXT
#define GL_HISTOGRAM_WIDTH_EXT 0x8026
#endif
#ifndef GL_HISTOGRAM_FORMAT_EXT
#define GL_HISTOGRAM_FORMAT_EXT 0x8027
#endif
#ifndef GL_HISTOGRAM_RED_SIZE_EXT
#define GL_HISTOGRAM_RED_SIZE_EXT 0x8028
#endif
#ifndef GL_HISTOGRAM_GREEN_SIZE_EXT
#define GL_HISTOGRAM_GREEN_SIZE_EXT 0x8029
#endif
#ifndef GL_HISTOGRAM_BLUE_SIZE_EXT
#define GL_HISTOGRAM_BLUE_SIZE_EXT 0x802A
#endif
#ifndef GL_HISTOGRAM_ALPHA_SIZE_EXT
#define GL_HISTOGRAM_ALPHA_SIZE_EXT 0x802B
#endif
#ifndef GL_HISTOGRAM_LUMINANCE_SIZE_EXT
#define GL_HISTOGRAM_LUMINANCE_SIZE_EXT 0x802C
#endif
#ifndef GL_HISTOGRAM_SINK_EXT
#define GL_HISTOGRAM_SINK_EXT 0x802D
#endif
#ifndef GL_MINMAX_EXT
#define GL_MINMAX_EXT 0x802E
#endif
#ifndef GL_MINMAX_FORMAT_EXT
#define GL_MINMAX_FORMAT_EXT 0x802F
#endif
#ifndef GL_MINMAX_SINK_EXT
#define GL_MINMAX_SINK_EXT 0x8030
#endif
#ifndef GL_TABLE_TOO_LARGE_EXT
#define GL_TABLE_TOO_LARGE_EXT 0x8031
#endif
#ifndef GL_CONVOLUTION_1D_EXT
#define GL_CONVOLUTION_1D_EXT 0x8010
#endif
#ifndef GL_CONVOLUTION_2D_EXT
#define GL_CONVOLUTION_2D_EXT 0x8011
#endif
#ifndef GL_SEPARABLE_2D_EXT
#define GL_SEPARABLE_2D_EXT 0x8012
#endif
#ifndef GL_CONVOLUTION_BORDER_MODE_EXT
#define GL_CONVOLUTION_BORDER_MODE_EXT 0x8013
#endif
#ifndef GL_CONVOLUTION_FILTER_SCALE_EXT
#define GL_CONVOLUTION_FILTER_SCALE_EXT 0x8014
#endif
#ifndef GL_CONVOLUTION_FILTER_BIAS_EXT
#define GL_CONVOLUTION_FILTER_BIAS_EXT 0x8015
#endif
#ifndef GL_REDUCE_EXT
#define GL_REDUCE_EXT 0x8016
#endif
#ifndef GL_CONVOLUTION_FORMAT_EXT
#define GL_CONVOLUTION_FORMAT_EXT 0x8017
#endif
#ifndef GL_CONVOLUTION_WIDTH_EXT
#define GL_CONVOLUTION_WIDTH_EXT 0x8018
#endif
#ifndef GL_CONVOLUTION_HEIGHT_EXT
#define GL_CONVOLUTION_HEIGHT_EXT 0x8019
#endif
#ifndef GL_MAX_CONVOLUTION_WIDTH_EXT
#define GL_MAX_CONVOLUTION_WIDTH_EXT 0x801A
#endif
#ifndef GL_MAX_CONVOLUTION_HEIGHT_EXT
#define GL_MAX_CONVOLUTION_HEIGHT_EXT 0x801B
#endif
#ifndef GL_POST_CONVOLUTION_RED_SCALE_EXT
#define GL_POST_CONVOLUTION_RED_SCALE_EXT 0x801C
#endif
#ifndef GL_POST_CONVOLUTION_GREEN_SCALE_EXT
#define GL_POST_CONVOLUTION_GREEN_SCALE_EXT 0x801D
#endif
#ifndef GL_POST_CONVOLUTION_BLUE_SCALE_EXT
#define GL_POST_CONVOLUTION_BLUE_SCALE_EXT 0x801E
#endif
#ifndef GL_POST_CONVOLUTION_ALPHA_SCALE_EXT
#define GL_POST_CONVOLUTION_ALPHA_SCALE_EXT 0x801F
#endif
#ifndef GL_POST_CONVOLUTION_RED_BIAS_EXT
#define GL_POST_CONVOLUTION_RED_BIAS_EXT 0x8020
#endif
#ifndef GL_POST_CONVOLUTION_GREEN_BIAS_EXT
#define GL_POST_CONVOLUTION_GREEN_BIAS_EXT 0x8021
#endif
#ifndef GL_POST_CONVOLUTION_BLUE_BIAS_EXT
#define GL_POST_CONVOLUTION_BLUE_BIAS_EXT 0x8022
#endif
#ifndef GL_POST_CONVOLUTION_ALPHA_BIAS_EXT
#define GL_POST_CONVOLUTION_ALPHA_BIAS_EXT 0x8023
#endif
#ifndef GL_COLOR_MATRIX_SGI
#define GL_COLOR_MATRIX_SGI 0x80B1
#endif
#ifndef GL_COLOR_MATRIX_STACK_DEPTH_SGI
#define GL_COLOR_MATRIX_STACK_DEPTH_SGI 0x80B2
#endif
#ifndef GL_MAX_COLOR_MATRIX_STACK_DEPTH_SGI
#define GL_MAX_COLOR_MATRIX_STACK_DEPTH_SGI 0x80B3
#endif
#ifndef GL_POST_COLOR_MATRIX_RED_SCALE_SGI
#define GL_POST_COLOR_MATRIX_RED_SCALE_SGI 0x80B4
#endif
#ifndef GL_POST_COLOR_MATRIX_GREEN_SCALE_SGI
#define GL_POST_COLOR_MATRIX_GREEN_SCALE_SGI 0x80B5
#endif
#ifndef GL_POST_COLOR_MATRIX_BLUE_SCALE_SGI
#define GL_POST_COLOR_MATRIX_BLUE_SCALE_SGI 0x80B6
#endif
#ifndef GL_POST_COLOR_MATRIX_ALPHA_SCALE_SGI
#define GL_POST_COLOR_MATRIX_ALPHA_SCALE_SGI 0x80B7
#endif
#ifndef GL_POST_COLOR_MATRIX_RED_BIAS_SGI
#define GL_POST_COLOR_MATRIX_RED_BIAS_SGI 0x80B8
#endif
#ifndef GL_POST_COLOR_MATRIX_GREEN_BIAS_SGI
#define GL_POST_COLOR_MATRIX_GREEN_BIAS_SGI 0x80B9
#endif
#ifndef GL_POST_COLOR_MATRIX_BLUE_BIAS_SGI
#define GL_POST_COLOR_MATRIX_BLUE_BIAS_SGI 0x80BA
#endif
#ifndef GL_POST_COLOR_MATRIX_ALPHA_BIAS_SGI
#define GL_POST_COLOR_MATRIX_ALPHA_BIAS_SGI 0x80BB
#endif
#ifndef GL_COLOR_TABLE_SGI
#define GL_COLOR_TABLE_SGI 0x80D0
#endif
#ifndef GL_POST_CONVOLUTION_COLOR_TABLE_SGI
#define GL_POST_CONVOLUTION_COLOR_TABLE_SGI 0x80D1
#endif
#ifndef GL_POST_COLOR_MATRIX_COLOR_TABLE_SGI
#define GL_POST_COLOR_MATRIX_COLOR_TABLE_SGI 0x80D2
#endif
#ifndef GL_PROXY_COLOR_TABLE_SGI
#define GL_PROXY_COLOR_TABLE_SGI 0x80D3
#endif
#ifndef GL_PROXY_POST_CONVOLUTION_COLOR_TABLE_SGI
#define GL_PROXY_POST_CONVOLUTION_COLOR_TABLE_SGI 0x80D4
#endif
#ifndef GL_PROXY_POST_COLOR_MATRIX_COLOR_TABLE_SGI
#define GL_PROXY_POST_COLOR_MATRIX_COLOR_TABLE_SGI 0x80D5
#endif
#ifndef GL_COLOR_TABLE_SCALE_SGI
#define GL_COLOR_TABLE_SCALE_SGI 0x80D6
#endif
#ifndef GL_COLOR_TABLE_BIAS_SGI
#define GL_COLOR_TABLE_BIAS_SGI 0x80D7
#endif
#ifndef GL_COLOR_TABLE_FORMAT_SGI
#define GL_COLOR_TABLE_FORMAT_SGI 0x80D8
#endif
#ifndef GL_COLOR_TABLE_WIDTH_SGI
#define GL_COLOR_TABLE_WIDTH_SGI 0x80D9
#endif
#ifndef GL_COLOR_TABLE_RED_SIZE_SGI
#define GL_COLOR_TABLE_RED_SIZE_SGI 0x80DA
#endif
#ifndef GL_COLOR_TABLE_GREEN_SIZE_SGI
#define GL_COLOR_TABLE_GREEN_SIZE_SGI 0x80DB
#endif
#ifndef GL_COLOR_TABLE_BLUE_SIZE_SGI
#define GL_COLOR_TABLE_BLUE_SIZE_SGI 0x80DC
#endif
#ifndef GL_COLOR_TABLE_ALPHA_SIZE_SGI
#define GL_COLOR_TABLE_ALPHA_SIZE_SGI 0x80DD
#endif
#ifndef GL_COLOR_TABLE_LUMINANCE_SIZE_SGI
#define GL_COLOR_TABLE_LUMINANCE_SIZE_SGI 0x80DE
#endif
#ifndef GL_COLOR_TABLE_INTENSITY_SIZE_SGI
#define GL_COLOR_TABLE_INTENSITY_SIZE_SGI 0x80DF
#endif
#ifndef GL_PIXEL_TEXTURE_SGIS
#define GL_PIXEL_TEXTURE_SGIS 0x8353
#endif
#ifndef GL_PIXEL_FRAGMENT_RGB_SOURCE_SGIS
#define GL_PIXEL_FRAGMENT_RGB_SOURCE_SGIS 0x8354
#endif
#ifndef GL_PIXEL_FRAGMENT_ALPHA_SOURCE_SGIS
#define GL_PIXEL_FRAGMENT_ALPHA_SOURCE_SGIS 0x8355
#endif
#ifndef GL_PIXEL_GROUP_COLOR_SGIS
#define GL_PIXEL_GROUP_COLOR_SGIS 0x8356
#endif
#ifndef GL_PIXEL_TEX_GEN_SGIX
#define GL_PIXEL_TEX_GEN_SGIX 0x8139
#endif
#ifndef GL_PIXEL_TEX_GEN_MODE_SGIX
#define GL_PIXEL_TEX_GEN_MODE_SGIX 0x832B
#endif
#ifndef GL_PACK_SKIP_VOLUMES_SGIS
#define GL_PACK_SKIP_VOLUMES_SGIS 0x8130
#endif
#ifndef GL_PACK_IMAGE_DEPTH_SGIS
#define GL_PACK_IMAGE_DEPTH_SGIS 0x8131
#endif
#ifndef GL_UNPACK_SKIP_VOLUMES_SGIS
#define GL_UNPACK_SKIP_VOLUMES_SGIS 0x8132
#endif
#ifndef GL_UNPACK_IMAGE_DEPTH_SGIS
#define GL_UNPACK_IMAGE_DEPTH_SGIS 0x8133
#endif
#ifndef GL_TEXTURE_4D_SGIS
#define GL_TEXTURE_4D_SGIS 0x8134
#endif
#ifndef GL_PROXY_TEXTURE_4D_SGIS
#define GL_PROXY_TEXTURE_4D_SGIS 0x8135
#endif
#ifndef GL_TEXTURE_4DSIZE_SGIS
#define GL_TEXTURE_4DSIZE_SGIS 0x8136
#endif
#ifndef GL_TEXTURE_WRAP_Q_SGIS
#define GL_TEXTURE_WRAP_Q_SGIS 0x8137
#endif
#ifndef GL_MAX_4D_TEXTURE_SIZE_SGIS
#define GL_MAX_4D_TEXTURE_SIZE_SGIS 0x8138
#endif
#ifndef GL_TEXTURE_4D_BINDING_SGIS
#define GL_TEXTURE_4D_BINDING_SGIS 0x814F
#endif
#ifndef GL_TEXTURE_COLOR_TABLE_SGI
#define GL_TEXTURE_COLOR_TABLE_SGI 0x80BC
#endif
#ifndef GL_PROXY_TEXTURE_COLOR_TABLE_SGI
#define GL_PROXY_TEXTURE_COLOR_TABLE_SGI 0x80BD
#endif
#ifndef GL_CMYK_EXT
#define GL_CMYK_EXT 0x800C
#endif
#ifndef GL_CMYKA_EXT
#define GL_CMYKA_EXT 0x800D
#endif
#ifndef GL_PACK_CMYK_HINT_EXT
#define GL_PACK_CMYK_HINT_EXT 0x800E
#endif
#ifndef GL_UNPACK_CMYK_HINT_EXT
#define GL_UNPACK_CMYK_HINT_EXT 0x800F
#endif
#ifndef GL_TEXTURE_PRIORITY_EXT
#define GL_TEXTURE_PRIORITY_EXT 0x8066
#endif
#ifndef GL_TEXTURE_RESIDENT_EXT
#define GL_TEXTURE_RESIDENT_EXT 0x8067
#endif
#ifndef GL_TEXTURE_1D_BINDING_EXT
#define GL_TEXTURE_1D_BINDING_EXT 0x8068
#endif
#ifndef GL_TEXTURE_2D_BINDING_EXT
#define GL_TEXTURE_2D_BINDING_EXT 0x8069
#endif
#ifndef GL_TEXTURE_3D_BINDING_EXT
#define GL_TEXTURE_3D_BINDING_EXT 0x806A
#endif
#ifndef GL_DETAIL_TEXTURE_2D_SGIS
#define GL_DETAIL_TEXTURE_2D_SGIS 0x8095
#endif
#ifndef GL_DETAIL_TEXTURE_2D_BINDING_SGIS
#define GL_DETAIL_TEXTURE_2D_BINDING_SGIS 0x8096
#endif
#ifndef GL_LINEAR_DETAIL_SGIS
#define GL_LINEAR_DETAIL_SGIS 0x8097
#endif
#ifndef GL_LINEAR_DETAIL_ALPHA_SGIS
#define GL_LINEAR_DETAIL_ALPHA_SGIS 0x8098
#endif
#ifndef GL_LINEAR_DETAIL_COLOR_SGIS
#define GL_LINEAR_DETAIL_COLOR_SGIS 0x8099
#endif
#ifndef GL_DETAIL_TEXTURE_LEVEL_SGIS
#define GL_DETAIL_TEXTURE_LEVEL_SGIS 0x809A
#endif
#ifndef GL_DETAIL_TEXTURE_MODE_SGIS
#define GL_DETAIL_TEXTURE_MODE_SGIS 0x809B
#endif
#ifndef GL_DETAIL_TEXTURE_FUNC_POINTS_SGIS
#define GL_DETAIL_TEXTURE_FUNC_POINTS_SGIS 0x809C
#endif
#ifndef GL_LINEAR_SHARPEN_SGIS
#define GL_LINEAR_SHARPEN_SGIS 0x80AD
#endif
#ifndef GL_LINEAR_SHARPEN_ALPHA_SGIS
#define GL_LINEAR_SHARPEN_ALPHA_SGIS 0x80AE
#endif
#ifndef GL_LINEAR_SHARPEN_COLOR_SGIS
#define GL_LINEAR_SHARPEN_COLOR_SGIS 0x80AF
#endif
#ifndef GL_SHARPEN_TEXTURE_FUNC_POINTS_SGIS
#define GL_SHARPEN_TEXTURE_FUNC_POINTS_SGIS 0x80B0
#endif
#ifndef GL_UNSIGNED_BYTE_3_3_2_EXT
#define GL_UNSIGNED_BYTE_3_3_2_EXT 0x8032
#endif
#ifndef GL_UNSIGNED_SHORT_4_4_4_4_EXT
#define GL_UNSIGNED_SHORT_4_4_4_4_EXT 0x8033
#endif
#ifndef GL_UNSIGNED_SHORT_5_5_5_1_EXT
#define GL_UNSIGNED_SHORT_5_5_5_1_EXT 0x8034
#endif
#ifndef GL_UNSIGNED_INT_8_8_8_8_EXT
#define GL_UNSIGNED_INT_8_8_8_8_EXT 0x8035
#endif
#ifndef GL_UNSIGNED_INT_10_10_10_2_EXT
#define GL_UNSIGNED_INT_10_10_10_2_EXT 0x8036
#endif
#ifndef GL_TEXTURE_MIN_LOD_SGIS
#define GL_TEXTURE_MIN_LOD_SGIS 0x813A
#endif
#ifndef GL_TEXTURE_MAX_LOD_SGIS
#define GL_TEXTURE_MAX_LOD_SGIS 0x813B
#endif
#ifndef GL_TEXTURE_BASE_LEVEL_SGIS
#define GL_TEXTURE_BASE_LEVEL_SGIS 0x813C
#endif
#ifndef GL_TEXTURE_MAX_LEVEL_SGIS
#define GL_TEXTURE_MAX_LEVEL_SGIS 0x813D
#endif
#ifndef GL_MULTISAMPLE_SGIS
#define GL_MULTISAMPLE_SGIS 0x809D
#endif
#ifndef GL_SAMPLE_ALPHA_TO_MASK_SGIS
#define GL_SAMPLE_ALPHA_TO_MASK_SGIS 0x809E
#endif
#ifndef GL_SAMPLE_ALPHA_TO_ONE_SGIS
#define GL_SAMPLE_ALPHA_TO_ONE_SGIS 0x809F
#endif
#ifndef GL_SAMPLE_MASK_SGIS
#define GL_SAMPLE_MASK_SGIS 0x80A0
#endif
#ifndef GL_1PASS_SGIS
#define GL_1PASS_SGIS 0x80A1
#endif
#ifndef GL_2PASS_0_SGIS
#define GL_2PASS_0_SGIS 0x80A2
#endif
#ifndef GL_2PASS_1_SGIS
#define GL_2PASS_1_SGIS 0x80A3
#endif
#ifndef GL_4PASS_0_SGIS
#define GL_4PASS_0_SGIS 0x80A4
#endif
#ifndef GL_4PASS_1_SGIS
#define GL_4PASS_1_SGIS 0x80A5
#endif
#ifndef GL_4PASS_2_SGIS
#define GL_4PASS_2_SGIS 0x80A6
#endif
#ifndef GL_4PASS_3_SGIS
#define GL_4PASS_3_SGIS 0x80A7
#endif
#ifndef GL_SAMPLE_BUFFERS_SGIS
#define GL_SAMPLE_BUFFERS_SGIS 0x80A8
#endif
#ifndef GL_SAMPLES_SGIS
#define GL_SAMPLES_SGIS 0x80A9
#endif
#ifndef GL_SAMPLE_MASK_VALUE_SGIS
#define GL_SAMPLE_MASK_VALUE_SGIS 0x80AA
#endif
#ifndef GL_SAMPLE_MASK_INVERT_SGIS
#define GL_SAMPLE_MASK_INVERT_SGIS 0x80AB
#endif
#ifndef GL_SAMPLE_PATTERN_SGIS
#define GL_SAMPLE_PATTERN_SGIS 0x80AC
#endif
#ifndef GL_RESCALE_NORMAL_EXT
#define GL_RESCALE_NORMAL_EXT 0x803A
#endif
#ifndef GL_VERTEX_ARRAY_EXT
#define GL_VERTEX_ARRAY_EXT 0x8074
#endif
#ifndef GL_NORMAL_ARRAY_EXT
#define GL_NORMAL_ARRAY_EXT 0x8075
#endif
#ifndef GL_COLOR_ARRAY_EXT
#define GL_COLOR_ARRAY_EXT 0x8076
#endif
#ifndef GL_INDEX_ARRAY_EXT
#define GL_INDEX_ARRAY_EXT 0x8077
#endif
#ifndef GL_TEXTURE_COORD_ARRAY_EXT
#define GL_TEXTURE_COORD_ARRAY_EXT 0x8078
#endif
#ifndef GL_EDGE_FLAG_ARRAY_EXT
#define GL_EDGE_FLAG_ARRAY_EXT 0x8079
#endif
#ifndef GL_VERTEX_ARRAY_SIZE_EXT
#define GL_VERTEX_ARRAY_SIZE_EXT 0x807A
#endif
#ifndef GL_VERTEX_ARRAY_TYPE_EXT
#define GL_VERTEX_ARRAY_TYPE_EXT 0x807B
#endif
#ifndef GL_VERTEX_ARRAY_STRIDE_EXT
#define GL_VERTEX_ARRAY_STRIDE_EXT 0x807C
#endif
#ifndef GL_VERTEX_ARRAY_COUNT_EXT
#define GL_VERTEX_ARRAY_COUNT_EXT 0x807D
#endif
#ifndef GL_NORMAL_ARRAY_TYPE_EXT
#define GL_NORMAL_ARRAY_TYPE_EXT 0x807E
#endif
#ifndef GL_NORMAL_ARRAY_STRIDE_EXT
#define GL_NORMAL_ARRAY_STRIDE_EXT 0x807F
#endif
#ifndef GL_NORMAL_ARRAY_COUNT_EXT
#define GL_NORMAL_ARRAY_COUNT_EXT 0x8080
#endif
#ifndef GL_COLOR_ARRAY_SIZE_EXT
#define GL_COLOR_ARRAY_SIZE_EXT 0x8081
#endif
#ifndef GL_COLOR_ARRAY_TYPE_EXT
#define GL_COLOR_ARRAY_TYPE_EXT 0x8082
#endif
#ifndef GL_COLOR_ARRAY_STRIDE_EXT
#define GL_COLOR_ARRAY_STRIDE_EXT 0x8083
#endif
#ifndef GL_COLOR_ARRAY_COUNT_EXT
#define GL_COLOR_ARRAY_COUNT_EXT 0x8084
#endif
#ifndef GL_INDEX_ARRAY_TYPE_EXT
#define GL_INDEX_ARRAY_TYPE_EXT 0x8085
#endif
#ifndef GL_INDEX_ARRAY_STRIDE_EXT
#define GL_INDEX_ARRAY_STRIDE_EXT 0x8086
#endif
#ifndef GL_INDEX_ARRAY_COUNT_EXT
#define GL_INDEX_ARRAY_COUNT_EXT 0x8087
#endif
#ifndef GL_TEXTURE_COORD_ARRAY_SIZE_EXT
#define GL_TEXTURE_COORD_ARRAY_SIZE_EXT 0x8088
#endif
#ifndef GL_TEXTURE_COORD_ARRAY_TYPE_EXT
#define GL_TEXTURE_COORD_ARRAY_TYPE_EXT 0x8089
#endif
#ifndef GL_TEXTURE_COORD_ARRAY_STRIDE_EXT
#define GL_TEXTURE_COORD_ARRAY_STRIDE_EXT 0x808A
#endif
#ifndef GL_TEXTURE_COORD_ARRAY_COUNT_EXT
#define GL_TEXTURE_COORD_ARRAY_COUNT_EXT 0x808B
#endif
#ifndef GL_EDGE_FLAG_ARRAY_STRIDE_EXT
#define GL_EDGE_FLAG_ARRAY_STRIDE_EXT 0x808C
#endif
#ifndef GL_EDGE_FLAG_ARRAY_COUNT_EXT
#define GL_EDGE_FLAG_ARRAY_COUNT_EXT 0x808D
#endif
#ifndef GL_VERTEX_ARRAY_POINTER_EXT
#define GL_VERTEX_ARRAY_POINTER_EXT 0x808E
#endif
#ifndef GL_NORMAL_ARRAY_POINTER_EXT
#define GL_NORMAL_ARRAY_POINTER_EXT 0x808F
#endif
#ifndef GL_COLOR_ARRAY_POINTER_EXT
#define GL_COLOR_ARRAY_POINTER_EXT 0x8090
#endif
#ifndef GL_INDEX_ARRAY_POINTER_EXT
#define GL_INDEX_ARRAY_POINTER_EXT 0x8091
#endif
#ifndef GL_TEXTURE_COORD_ARRAY_POINTER_EXT
#define GL_TEXTURE_COORD_ARRAY_POINTER_EXT 0x8092
#endif
#ifndef GL_EDGE_FLAG_ARRAY_POINTER_EXT
#define GL_EDGE_FLAG_ARRAY_POINTER_EXT 0x8093
#endif
#ifndef GL_GENERATE_MIPMAP_SGIS
#define GL_GENERATE_MIPMAP_SGIS 0x8191
#endif
#ifndef GL_GENERATE_MIPMAP_HINT_SGIS
#define GL_GENERATE_MIPMAP_HINT_SGIS 0x8192
#endif
#ifndef GL_LINEAR_CLIPMAP_LINEAR_SGIX
#define GL_LINEAR_CLIPMAP_LINEAR_SGIX 0x8170
#endif
#ifndef GL_TEXTURE_CLIPMAP_CENTER_SGIX
#define GL_TEXTURE_CLIPMAP_CENTER_SGIX 0x8171
#endif
#ifndef GL_TEXTURE_CLIPMAP_FRAME_SGIX
#define GL_TEXTURE_CLIPMAP_FRAME_SGIX 0x8172
#endif
#ifndef GL_TEXTURE_CLIPMAP_OFFSET_SGIX
#define GL_TEXTURE_CLIPMAP_OFFSET_SGIX 0x8173
#endif
#ifndef GL_TEXTURE_CLIPMAP_VIRTUAL_DEPTH_SGIX
#define GL_TEXTURE_CLIPMAP_VIRTUAL_DEPTH_SGIX 0x8174
#endif
#ifndef GL_TEXTURE_CLIPMAP_LOD_OFFSET_SGIX
#define GL_TEXTURE_CLIPMAP_LOD_OFFSET_SGIX 0x8175
#endif
#ifndef GL_TEXTURE_CLIPMAP_DEPTH_SGIX
#define GL_TEXTURE_CLIPMAP_DEPTH_SGIX 0x8176
#endif
#ifndef GL_MAX_CLIPMAP_DEPTH_SGIX
#define GL_MAX_CLIPMAP_DEPTH_SGIX 0x8177
#endif
#ifndef GL_MAX_CLIPMAP_VIRTUAL_DEPTH_SGIX
#define GL_MAX_CLIPMAP_VIRTUAL_DEPTH_SGIX 0x8178
#endif
#ifndef GL_NEAREST_CLIPMAP_NEAREST_SGIX
#define GL_NEAREST_CLIPMAP_NEAREST_SGIX 0x844D
#endif
#ifndef GL_NEAREST_CLIPMAP_LINEAR_SGIX
#define GL_NEAREST_CLIPMAP_LINEAR_SGIX 0x844E
#endif
#ifndef GL_LINEAR_CLIPMAP_NEAREST_SGIX
#define GL_LINEAR_CLIPMAP_NEAREST_SGIX 0x844F
#endif
#ifndef GL_TEXTURE_COMPARE_SGIX
#define GL_TEXTURE_COMPARE_SGIX 0x819A
#endif
#ifndef GL_TEXTURE_COMPARE_OPERATOR_SGIX
#define GL_TEXTURE_COMPARE_OPERATOR_SGIX 0x819B
#endif
#ifndef GL_TEXTURE_LEQUAL_R_SGIX
#define GL_TEXTURE_LEQUAL_R_SGIX 0x819C
#endif
#ifndef GL_TEXTURE_GEQUAL_R_SGIX
#define GL_TEXTURE_GEQUAL_R_SGIX 0x819D
#endif
#ifndef GL_CLAMP_TO_EDGE_SGIS
#define GL_CLAMP_TO_EDGE_SGIS 0x812F
#endif
#ifndef GL_CLAMP_TO_BORDER_SGIS
#define GL_CLAMP_TO_BORDER_SGIS 0x812D
#endif
#ifndef GL_FUNC_ADD_EXT
#define GL_FUNC_ADD_EXT 0x8006
#endif
#ifndef GL_MIN_EXT
#define GL_MIN_EXT 0x8007
#endif
#ifndef GL_MAX_EXT
#define GL_MAX_EXT 0x8008
#endif
#ifndef GL_BLEND_EQUATION_EXT
#define GL_BLEND_EQUATION_EXT 0x8009
#endif
#ifndef GL_FUNC_SUBTRACT_EXT
#define GL_FUNC_SUBTRACT_EXT 0x800A
#endif
#ifndef GL_FUNC_REVERSE_SUBTRACT_EXT
#define GL_FUNC_REVERSE_SUBTRACT_EXT 0x800B
#endif
#ifndef GL_INTERLACE_SGIX
#define GL_INTERLACE_SGIX 0x8094
#endif
#ifndef GL_PIXEL_TILE_BEST_ALIGNMENT_SGIX
#define GL_PIXEL_TILE_BEST_ALIGNMENT_SGIX 0x813E
#endif
#ifndef GL_PIXEL_TILE_CACHE_INCREMENT_SGIX
#define GL_PIXEL_TILE_CACHE_INCREMENT_SGIX 0x813F
#endif
#ifndef GL_PIXEL_TILE_WIDTH_SGIX
#define GL_PIXEL_TILE_WIDTH_SGIX 0x8140
#endif
#ifndef GL_PIXEL_TILE_HEIGHT_SGIX
#define GL_PIXEL_TILE_HEIGHT_SGIX 0x8141
#endif
#ifndef GL_PIXEL_TILE_GRID_WIDTH_SGIX
#define GL_PIXEL_TILE_GRID_WIDTH_SGIX 0x8142
#endif
#ifndef GL_PIXEL_TILE_GRID_HEIGHT_SGIX
#define GL_PIXEL_TILE_GRID_HEIGHT_SGIX 0x8143
#endif
#ifndef GL_PIXEL_TILE_GRID_DEPTH_SGIX
#define GL_PIXEL_TILE_GRID_DEPTH_SGIX 0x8144
#endif
#ifndef GL_PIXEL_TILE_CACHE_SIZE_SGIX
#define GL_PIXEL_TILE_CACHE_SIZE_SGIX 0x8145
#endif
#ifndef GL_DUAL_ALPHA4_SGIS
#define GL_DUAL_ALPHA4_SGIS 0x8110
#endif
#ifndef GL_DUAL_ALPHA8_SGIS
#define GL_DUAL_ALPHA8_SGIS 0x8111
#endif
#ifndef GL_DUAL_ALPHA12_SGIS
#define GL_DUAL_ALPHA12_SGIS 0x8112
#endif
#ifndef GL_DUAL_ALPHA16_SGIS
#define GL_DUAL_ALPHA16_SGIS 0x8113
#endif
#ifndef GL_DUAL_LUMINANCE4_SGIS
#define GL_DUAL_LUMINANCE4_SGIS 0x8114
#endif
#ifndef GL_DUAL_LUMINANCE8_SGIS
#define GL_DUAL_LUMINANCE8_SGIS 0x8115
#endif
#ifndef GL_DUAL_LUMINANCE12_SGIS
#define GL_DUAL_LUMINANCE12_SGIS 0x8116
#endif
#ifndef GL_DUAL_LUMINANCE16_SGIS
#define GL_DUAL_LUMINANCE16_SGIS 0x8117
#endif
#ifndef GL_DUAL_INTENSITY4_SGIS
#define GL_DUAL_INTENSITY4_SGIS 0x8118
#endif
#ifndef GL_DUAL_INTENSITY8_SGIS
#define GL_DUAL_INTENSITY8_SGIS 0x8119
#endif
#ifndef GL_DUAL_INTENSITY12_SGIS
#define GL_DUAL_INTENSITY12_SGIS 0x811A
#endif
#ifndef GL_DUAL_INTENSITY16_SGIS
#define GL_DUAL_INTENSITY16_SGIS 0x811B
#endif
#ifndef GL_DUAL_LUMINANCE_ALPHA4_SGIS
#define GL_DUAL_LUMINANCE_ALPHA4_SGIS 0x811C
#endif
#ifndef GL_DUAL_LUMINANCE_ALPHA8_SGIS
#define GL_DUAL_LUMINANCE_ALPHA8_SGIS 0x811D
#endif
#ifndef GL_QUAD_ALPHA4_SGIS
#define GL_QUAD_ALPHA4_SGIS 0x811E
#endif
#ifndef GL_QUAD_ALPHA8_SGIS
#define GL_QUAD_ALPHA8_SGIS 0x811F
#endif
#ifndef GL_QUAD_LUMINANCE4_SGIS
#define GL_QUAD_LUMINANCE4_SGIS 0x8120
#endif
#ifndef GL_QUAD_LUMINANCE8_SGIS
#define GL_QUAD_LUMINANCE8_SGIS 0x8121
#endif
#ifndef GL_QUAD_INTENSITY4_SGIS
#define GL_QUAD_INTENSITY4_SGIS 0x8122
#endif
#ifndef GL_QUAD_INTENSITY8_SGIS
#define GL_QUAD_INTENSITY8_SGIS 0x8123
#endif
#ifndef GL_DUAL_TEXTURE_SELECT_SGIS
#define GL_DUAL_TEXTURE_SELECT_SGIS 0x8124
#endif
#ifndef GL_QUAD_TEXTURE_SELECT_SGIS
#define GL_QUAD_TEXTURE_SELECT_SGIS 0x8125
#endif
#ifndef GL_SPRITE_SGIX
#define GL_SPRITE_SGIX 0x8148
#endif
#ifndef GL_SPRITE_MODE_SGIX
#define GL_SPRITE_MODE_SGIX 0x8149
#endif
#ifndef GL_SPRITE_AXIS_SGIX
#define GL_SPRITE_AXIS_SGIX 0x814A
#endif
#ifndef GL_SPRITE_TRANSLATION_SGIX
#define GL_SPRITE_TRANSLATION_SGIX 0x814B
#endif
#ifndef GL_SPRITE_AXIAL_SGIX
#define GL_SPRITE_AXIAL_SGIX 0x814C
#endif
#ifndef GL_SPRITE_OBJECT_ALIGNED_SGIX
#define GL_SPRITE_OBJECT_ALIGNED_SGIX 0x814D
#endif
#ifndef GL_SPRITE_EYE_ALIGNED_SGIX
#define GL_SPRITE_EYE_ALIGNED_SGIX 0x814E
#endif
#ifndef GL_TEXTURE_MULTI_BUFFER_HINT_SGIX
#define GL_TEXTURE_MULTI_BUFFER_HINT_SGIX 0x812E
#endif
#ifndef GL_POINT_SIZE_MIN_EXT
#define GL_POINT_SIZE_MIN_EXT 0x8126
#endif
#ifndef GL_POINT_SIZE_MAX_EXT
#define GL_POINT_SIZE_MAX_EXT 0x8127
#endif
#ifndef GL_POINT_FADE_THRESHOLD_SIZE_EXT
#define GL_POINT_FADE_THRESHOLD_SIZE_EXT 0x8128
#endif
#ifndef GL_DISTANCE_ATTENUATION_EXT
#define GL_DISTANCE_ATTENUATION_EXT 0x8129
#endif
#ifndef GL_POINT_SIZE_MIN_SGIS
#define GL_POINT_SIZE_MIN_SGIS 0x8126
#endif
#ifndef GL_POINT_SIZE_MAX_SGIS
#define GL_POINT_SIZE_MAX_SGIS 0x8127
#endif
#ifndef GL_POINT_FADE_THRESHOLD_SIZE_SGIS
#define GL_POINT_FADE_THRESHOLD_SIZE_SGIS 0x8128
#endif
#ifndef GL_DISTANCE_ATTENUATION_SGIS
#define GL_DISTANCE_ATTENUATION_SGIS 0x8129
#endif
#ifndef GL_INSTRUMENT_BUFFER_POINTER_SGIX
#define GL_INSTRUMENT_BUFFER_POINTER_SGIX 0x8180
#endif
#ifndef GL_INSTRUMENT_MEASUREMENTS_SGIX
#define GL_INSTRUMENT_MEASUREMENTS_SGIX 0x8181
#endif
#ifndef GL_POST_TEXTURE_FILTER_BIAS_SGIX
#define GL_POST_TEXTURE_FILTER_BIAS_SGIX 0x8179
#endif
#ifndef GL_POST_TEXTURE_FILTER_SCALE_SGIX
#define GL_POST_TEXTURE_FILTER_SCALE_SGIX 0x817A
#endif
#ifndef GL_POST_TEXTURE_FILTER_BIAS_RANGE_SGIX
#define GL_POST_TEXTURE_FILTER_BIAS_RANGE_SGIX 0x817B
#endif
#ifndef GL_POST_TEXTURE_FILTER_SCALE_RANGE_SGIX
#define GL_POST_TEXTURE_FILTER_SCALE_RANGE_SGIX 0x817C
#endif
#ifndef GL_FRAMEZOOM_SGIX
#define GL_FRAMEZOOM_SGIX 0x818B
#endif
#ifndef GL_FRAMEZOOM_FACTOR_SGIX
#define GL_FRAMEZOOM_FACTOR_SGIX 0x818C
#endif
#ifndef GL_MAX_FRAMEZOOM_FACTOR_SGIX
#define GL_MAX_FRAMEZOOM_FACTOR_SGIX 0x818D
#endif
#ifndef GL_TEXTURE_DEFORMATION_BIT_SGIX
#define GL_TEXTURE_DEFORMATION_BIT_SGIX 0x00000001
#endif
#ifndef GL_GEOMETRY_DEFORMATION_BIT_SGIX
#define GL_GEOMETRY_DEFORMATION_BIT_SGIX 0x00000002
#endif
#ifndef GL_GEOMETRY_DEFORMATION_SGIX
#define GL_GEOMETRY_DEFORMATION_SGIX 0x8194
#endif
#ifndef GL_TEXTURE_DEFORMATION_SGIX
#define GL_TEXTURE_DEFORMATION_SGIX 0x8195
#endif
#ifndef GL_DEFORMATIONS_MASK_SGIX
#define GL_DEFORMATIONS_MASK_SGIX 0x8196
#endif
#ifndef GL_MAX_DEFORMATION_ORDER_SGIX
#define GL_MAX_DEFORMATION_ORDER_SGIX 0x8197
#endif
#ifndef GL_REFERENCE_PLANE_SGIX
#define GL_REFERENCE_PLANE_SGIX 0x817D
#endif
#ifndef GL_REFERENCE_PLANE_EQUATION_SGIX
#define GL_REFERENCE_PLANE_EQUATION_SGIX 0x817E
#endif
#ifndef GL_DEPTH_COMPONENT16_SGIX
#define GL_DEPTH_COMPONENT16_SGIX 0x81A5
#endif
#ifndef GL_DEPTH_COMPONENT24_SGIX
#define GL_DEPTH_COMPONENT24_SGIX 0x81A6
#endif
#ifndef GL_DEPTH_COMPONENT32_SGIX
#define GL_DEPTH_COMPONENT32_SGIX 0x81A7
#endif
#ifndef GL_FOG_FUNC_SGIS
#define GL_FOG_FUNC_SGIS 0x812A
#endif
#ifndef GL_FOG_FUNC_POINTS_SGIS
#define GL_FOG_FUNC_POINTS_SGIS 0x812B
#endif
#ifndef GL_MAX_FOG_FUNC_POINTS_SGIS
#define GL_MAX_FOG_FUNC_POINTS_SGIS 0x812C
#endif
#ifndef GL_FOG_OFFSET_SGIX
#define GL_FOG_OFFSET_SGIX 0x8198
#endif
#ifndef GL_FOG_OFFSET_VALUE_SGIX
#define GL_FOG_OFFSET_VALUE_SGIX 0x8199
#endif
#ifndef GL_IMAGE_SCALE_X_HP
#define GL_IMAGE_SCALE_X_HP 0x8155
#endif
#ifndef GL_IMAGE_SCALE_Y_HP
#define GL_IMAGE_SCALE_Y_HP 0x8156
#endif
#ifndef GL_IMAGE_TRANSLATE_X_HP
#define GL_IMAGE_TRANSLATE_X_HP 0x8157
#endif
#ifndef GL_IMAGE_TRANSLATE_Y_HP
#define GL_IMAGE_TRANSLATE_Y_HP 0x8158
#endif
#ifndef GL_IMAGE_ROTATE_ANGLE_HP
#define GL_IMAGE_ROTATE_ANGLE_HP 0x8159
#endif
#ifndef GL_IMAGE_ROTATE_ORIGIN_X_HP
#define GL_IMAGE_ROTATE_ORIGIN_X_HP 0x815A
#endif
#ifndef GL_IMAGE_ROTATE_ORIGIN_Y_HP
#define GL_IMAGE_ROTATE_ORIGIN_Y_HP 0x815B
#endif
#ifndef GL_IMAGE_MAG_FILTER_HP
#define GL_IMAGE_MAG_FILTER_HP 0x815C
#endif
#ifndef GL_IMAGE_MIN_FILTER_HP
#define GL_IMAGE_MIN_FILTER_HP 0x815D
#endif
#ifndef GL_IMAGE_CUBIC_WEIGHT_HP
#define GL_IMAGE_CUBIC_WEIGHT_HP 0x815E
#endif
#ifndef GL_CUBIC_HP
#define GL_CUBIC_HP 0x815F
#endif
#ifndef GL_AVERAGE_HP
#define GL_AVERAGE_HP 0x8160
#endif
#ifndef GL_IMAGE_TRANSFORM_2D_HP
#define GL_IMAGE_TRANSFORM_2D_HP 0x8161
#endif
#ifndef GL_POST_IMAGE_TRANSFORM_COLOR_TABLE_HP
#define GL_POST_IMAGE_TRANSFORM_COLOR_TABLE_HP 0x8162
#endif
#ifndef GL_PROXY_POST_IMAGE_TRANSFORM_COLOR_TABLE_HP
#define GL_PROXY_POST_IMAGE_TRANSFORM_COLOR_TABLE_HP 0x8163
#endif
#ifndef GL_IGNORE_BORDER_HP
#define GL_IGNORE_BORDER_HP 0x8150
#endif
#ifndef GL_CONSTANT_BORDER_HP
#define GL_CONSTANT_BORDER_HP 0x8151
#endif
#ifndef GL_REPLICATE_BORDER_HP
#define GL_REPLICATE_BORDER_HP 0x8153
#endif
#ifndef GL_CONVOLUTION_BORDER_COLOR_HP
#define GL_CONVOLUTION_BORDER_COLOR_HP 0x8154
#endif
#ifndef GL_TEXTURE_ENV_BIAS_SGIX
#define GL_TEXTURE_ENV_BIAS_SGIX 0x80BE
#endif
#ifndef GL_VERTEX_DATA_HINT_PGI
#define GL_VERTEX_DATA_HINT_PGI 0x1A22A
#endif
#ifndef GL_VERTEX_CONSISTENT_HINT_PGI
#define GL_VERTEX_CONSISTENT_HINT_PGI 0x1A22B
#endif
#ifndef GL_MATERIAL_SIDE_HINT_PGI
#define GL_MATERIAL_SIDE_HINT_PGI 0x1A22C
#endif
#ifndef GL_MAX_VERTEX_HINT_PGI
#define GL_MAX_VERTEX_HINT_PGI 0x1A22D
#endif
#ifndef GL_COLOR3_BIT_PGI
#define GL_COLOR3_BIT_PGI 0x00010000
#endif
#ifndef GL_COLOR4_BIT_PGI
#define GL_COLOR4_BIT_PGI 0x00020000
#endif
#ifndef GL_EDGEFLAG_BIT_PGI
#define GL_EDGEFLAG_BIT_PGI 0x00040000
#endif
#ifndef GL_INDEX_BIT_PGI
#define GL_INDEX_BIT_PGI 0x00080000
#endif
#ifndef GL_MAT_AMBIENT_BIT_PGI
#define GL_MAT_AMBIENT_BIT_PGI 0x00100000
#endif
#ifndef GL_MAT_AMBIENT_AND_DIFFUSE_BIT_PGI
#define GL_MAT_AMBIENT_AND_DIFFUSE_BIT_PGI 0x00200000
#endif
#ifndef GL_MAT_DIFFUSE_BIT_PGI
#define GL_MAT_DIFFUSE_BIT_PGI 0x00400000
#endif
#ifndef GL_MAT_EMISSION_BIT_PGI
#define GL_MAT_EMISSION_BIT_PGI 0x00800000
#endif
#ifndef GL_MAT_COLOR_INDEXES_BIT_PGI
#define GL_MAT_COLOR_INDEXES_BIT_PGI 0x01000000
#endif
#ifndef GL_MAT_SHININESS_BIT_PGI
#define GL_MAT_SHININESS_BIT_PGI 0x02000000
#endif
#ifndef GL_MAT_SPECULAR_BIT_PGI
#define GL_MAT_SPECULAR_BIT_PGI 0x04000000
#endif
#ifndef GL_NORMAL_BIT_PGI
#define GL_NORMAL_BIT_PGI 0x08000000
#endif
#ifndef GL_TEXCOORD1_BIT_PGI
#define GL_TEXCOORD1_BIT_PGI 0x10000000
#endif
#ifndef GL_TEXCOORD2_BIT_PGI
#define GL_TEXCOORD2_BIT_PGI 0x20000000
#endif
#ifndef GL_TEXCOORD3_BIT_PGI
#define GL_TEXCOORD3_BIT_PGI 0x40000000
#endif
#ifndef GL_TEXCOORD4_BIT_PGI
#define GL_TEXCOORD4_BIT_PGI 0x80000000
#endif
#ifndef GL_VERTEX23_BIT_PGI
#define GL_VERTEX23_BIT_PGI 0x00000004
#endif
#ifndef GL_VERTEX4_BIT_PGI
#define GL_VERTEX4_BIT_PGI 0x00000008
#endif
#ifndef GL_PREFER_DOUBLEBUFFER_HINT_PGI
#define GL_PREFER_DOUBLEBUFFER_HINT_PGI 0x1A1F8
#endif
#ifndef GL_CONSERVE_MEMORY_HINT_PGI
#define GL_CONSERVE_MEMORY_HINT_PGI 0x1A1FD
#endif
#ifndef GL_RECLAIM_MEMORY_HINT_PGI
#define GL_RECLAIM_MEMORY_HINT_PGI 0x1A1FE
#endif
#ifndef GL_NATIVE_GRAPHICS_HANDLE_PGI
#define GL_NATIVE_GRAPHICS_HANDLE_PGI 0x1A202
#endif
#ifndef GL_NATIVE_GRAPHICS_BEGIN_HINT_PGI
#define GL_NATIVE_GRAPHICS_BEGIN_HINT_PGI 0x1A203
#endif
#ifndef GL_NATIVE_GRAPHICS_END_HINT_PGI
#define GL_NATIVE_GRAPHICS_END_HINT_PGI 0x1A204
#endif
#ifndef GL_ALWAYS_FAST_HINT_PGI
#define GL_ALWAYS_FAST_HINT_PGI 0x1A20C
#endif
#ifndef GL_ALWAYS_SOFT_HINT_PGI
#define GL_ALWAYS_SOFT_HINT_PGI 0x1A20D
#endif
#ifndef GL_ALLOW_DRAW_OBJ_HINT_PGI
#define GL_ALLOW_DRAW_OBJ_HINT_PGI 0x1A20E
#endif
#ifndef GL_ALLOW_DRAW_WIN_HINT_PGI
#define GL_ALLOW_DRAW_WIN_HINT_PGI 0x1A20F
#endif
#ifndef GL_ALLOW_DRAW_FRG_HINT_PGI
#define GL_ALLOW_DRAW_FRG_HINT_PGI 0x1A210
#endif
#ifndef GL_ALLOW_DRAW_MEM_HINT_PGI
#define GL_ALLOW_DRAW_MEM_HINT_PGI 0x1A211
#endif
#ifndef GL_STRICT_DEPTHFUNC_HINT_PGI
#define GL_STRICT_DEPTHFUNC_HINT_PGI 0x1A216
#endif
#ifndef GL_STRICT_LIGHTING_HINT_PGI
#define GL_STRICT_LIGHTING_HINT_PGI 0x1A217
#endif
#ifndef GL_STRICT_SCISSOR_HINT_PGI
#define GL_STRICT_SCISSOR_HINT_PGI 0x1A218
#endif
#ifndef GL_FULL_STIPPLE_HINT_PGI
#define GL_FULL_STIPPLE_HINT_PGI 0x1A219
#endif
#ifndef GL_CLIP_NEAR_HINT_PGI
#define GL_CLIP_NEAR_HINT_PGI 0x1A220
#endif
#ifndef GL_CLIP_FAR_HINT_PGI
#define GL_CLIP_FAR_HINT_PGI 0x1A221
#endif
#ifndef GL_WIDE_LINE_HINT_PGI
#define GL_WIDE_LINE_HINT_PGI 0x1A222
#endif
#ifndef GL_BACK_NORMALS_HINT_PGI
#define GL_BACK_NORMALS_HINT_PGI 0x1A223
#endif
#ifndef GL_COLOR_INDEX1_EXT
#define GL_COLOR_INDEX1_EXT 0x80E2
#endif
#ifndef GL_COLOR_INDEX2_EXT
#define GL_COLOR_INDEX2_EXT 0x80E3
#endif
#ifndef GL_COLOR_INDEX4_EXT
#define GL_COLOR_INDEX4_EXT 0x80E4
#endif
#ifndef GL_COLOR_INDEX8_EXT
#define GL_COLOR_INDEX8_EXT 0x80E5
#endif
#ifndef GL_COLOR_INDEX12_EXT
#define GL_COLOR_INDEX12_EXT 0x80E6
#endif
#ifndef GL_COLOR_INDEX16_EXT
#define GL_COLOR_INDEX16_EXT 0x80E7
#endif
#ifndef GL_TEXTURE_INDEX_SIZE_EXT
#define GL_TEXTURE_INDEX_SIZE_EXT 0x80ED
#endif
#ifndef GL_CLIP_VOLUME_CLIPPING_HINT_EXT
#define GL_CLIP_VOLUME_CLIPPING_HINT_EXT 0x80F0
#endif
#ifndef GL_LIST_PRIORITY_SGIX
#define GL_LIST_PRIORITY_SGIX 0x8182
#endif
#ifndef GL_IR_INSTRUMENT1_SGIX
#define GL_IR_INSTRUMENT1_SGIX 0x817F
#endif
#ifndef GL_CALLIGRAPHIC_FRAGMENT_SGIX
#define GL_CALLIGRAPHIC_FRAGMENT_SGIX 0x8183
#endif
#ifndef GL_TEXTURE_LOD_BIAS_S_SGIX
#define GL_TEXTURE_LOD_BIAS_S_SGIX 0x818E
#endif
#ifndef GL_TEXTURE_LOD_BIAS_T_SGIX
#define GL_TEXTURE_LOD_BIAS_T_SGIX 0x818F
#endif
#ifndef GL_TEXTURE_LOD_BIAS_R_SGIX
#define GL_TEXTURE_LOD_BIAS_R_SGIX 0x8190
#endif
#ifndef GL_SHADOW_AMBIENT_SGIX
#define GL_SHADOW_AMBIENT_SGIX 0x80BF
#endif
#ifndef GL_INDEX_MATERIAL_EXT
#define GL_INDEX_MATERIAL_EXT 0x81B8
#endif
#ifndef GL_INDEX_MATERIAL_PARAMETER_EXT
#define GL_INDEX_MATERIAL_PARAMETER_EXT 0x81B9
#endif
#ifndef GL_INDEX_MATERIAL_FACE_EXT
#define GL_INDEX_MATERIAL_FACE_EXT 0x81BA
#endif
#ifndef GL_INDEX_TEST_EXT
#define GL_INDEX_TEST_EXT 0x81B5
#endif
#ifndef GL_INDEX_TEST_FUNC_EXT
#define GL_INDEX_TEST_FUNC_EXT 0x81B6
#endif
#ifndef GL_INDEX_TEST_REF_EXT
#define GL_INDEX_TEST_REF_EXT 0x81B7
#endif
#ifndef GL_IUI_V2F_EXT
#define GL_IUI_V2F_EXT 0x81AD
#endif
#ifndef GL_IUI_V3F_EXT
#define GL_IUI_V3F_EXT 0x81AE
#endif
#ifndef GL_IUI_N3F_V2F_EXT
#define GL_IUI_N3F_V2F_EXT 0x81AF
#endif
#ifndef GL_IUI_N3F_V3F_EXT
#define GL_IUI_N3F_V3F_EXT 0x81B0
#endif
#ifndef GL_T2F_IUI_V2F_EXT
#define GL_T2F_IUI_V2F_EXT 0x81B1
#endif
#ifndef GL_T2F_IUI_V3F_EXT
#define GL_T2F_IUI_V3F_EXT 0x81B2
#endif
#ifndef GL_T2F_IUI_N3F_V2F_EXT
#define GL_T2F_IUI_N3F_V2F_EXT 0x81B3
#endif
#ifndef GL_T2F_IUI_N3F_V3F_EXT
#define GL_T2F_IUI_N3F_V3F_EXT 0x81B4
#endif
#ifndef GL_ARRAY_ELEMENT_LOCK_FIRST_EXT
#define GL_ARRAY_ELEMENT_LOCK_FIRST_EXT 0x81A8
#endif
#ifndef GL_ARRAY_ELEMENT_LOCK_COUNT_EXT
#define GL_ARRAY_ELEMENT_LOCK_COUNT_EXT 0x81A9
#endif
#ifndef GL_CULL_VERTEX_EXT
#define GL_CULL_VERTEX_EXT 0x81AA
#endif
#ifndef GL_CULL_VERTEX_EYE_POSITION_EXT
#define GL_CULL_VERTEX_EYE_POSITION_EXT 0x81AB
#endif
#ifndef GL_CULL_VERTEX_OBJECT_POSITION_EXT
#define GL_CULL_VERTEX_OBJECT_POSITION_EXT 0x81AC
#endif
#ifndef GL_YCRCB_422_SGIX
#define GL_YCRCB_422_SGIX 0x81BB
#endif
#ifndef GL_YCRCB_444_SGIX
#define GL_YCRCB_444_SGIX 0x81BC
#endif
#ifndef GL_FRAGMENT_LIGHTING_SGIX
#define GL_FRAGMENT_LIGHTING_SGIX 0x8400
#endif
#ifndef GL_FRAGMENT_COLOR_MATERIAL_SGIX
#define GL_FRAGMENT_COLOR_MATERIAL_SGIX 0x8401
#endif
#ifndef GL_FRAGMENT_COLOR_MATERIAL_FACE_SGIX
#define GL_FRAGMENT_COLOR_MATERIAL_FACE_SGIX 0x8402
#endif
#ifndef GL_FRAGMENT_COLOR_MATERIAL_PARAMETER_SGIX
#define GL_FRAGMENT_COLOR_MATERIAL_PARAMETER_SGIX 0x8403
#endif
#ifndef GL_MAX_FRAGMENT_LIGHTS_SGIX
#define GL_MAX_FRAGMENT_LIGHTS_SGIX 0x8404
#endif
#ifndef GL_MAX_ACTIVE_LIGHTS_SGIX
#define GL_MAX_ACTIVE_LIGHTS_SGIX 0x8405
#endif
#ifndef GL_CURRENT_RASTER_NORMAL_SGIX
#define GL_CURRENT_RASTER_NORMAL_SGIX 0x8406
#endif
#ifndef GL_LIGHT_ENV_MODE_SGIX
#define GL_LIGHT_ENV_MODE_SGIX 0x8407
#endif
#ifndef GL_FRAGMENT_LIGHT_MODEL_LOCAL_VIEWER_SGIX
#define GL_FRAGMENT_LIGHT_MODEL_LOCAL_VIEWER_SGIX 0x8408
#endif
#ifndef GL_FRAGMENT_LIGHT_MODEL_TWO_SIDE_SGIX
#define GL_FRAGMENT_LIGHT_MODEL_TWO_SIDE_SGIX 0x8409
#endif
#ifndef GL_FRAGMENT_LIGHT_MODEL_AMBIENT_SGIX
#define GL_FRAGMENT_LIGHT_MODEL_AMBIENT_SGIX 0x840A
#endif
#ifndef GL_FRAGMENT_LIGHT_MODEL_NORMAL_INTERPOLATION_SGIX
#define GL_FRAGMENT_LIGHT_MODEL_NORMAL_INTERPOLATION_SGIX 0x840B
#endif
#ifndef GL_FRAGMENT_LIGHT0_SGIX
#define GL_FRAGMENT_LIGHT0_SGIX 0x840C
#endif
#ifndef GL_FRAGMENT_LIGHT1_SGIX
#define GL_FRAGMENT_LIGHT1_SGIX 0x840D
#endif
#ifndef GL_FRAGMENT_LIGHT2_SGIX
#define GL_FRAGMENT_LIGHT2_SGIX 0x840E
#endif
#ifndef GL_FRAGMENT_LIGHT3_SGIX
#define GL_FRAGMENT_LIGHT3_SGIX 0x840F
#endif
#ifndef GL_FRAGMENT_LIGHT4_SGIX
#define GL_FRAGMENT_LIGHT4_SGIX 0x8410
#endif
#ifndef GL_FRAGMENT_LIGHT5_SGIX
#define GL_FRAGMENT_LIGHT5_SGIX 0x8411
#endif
#ifndef GL_FRAGMENT_LIGHT6_SGIX
#define GL_FRAGMENT_LIGHT6_SGIX 0x8412
#endif
#ifndef GL_FRAGMENT_LIGHT7_SGIX
#define GL_FRAGMENT_LIGHT7_SGIX 0x8413
#endif
#ifndef GL_RASTER_POSITION_UNCLIPPED_IBM
#define GL_RASTER_POSITION_UNCLIPPED_IBM 0x19262
#endif
#ifndef GL_TEXTURE_LIGHTING_MODE_HP
#define GL_TEXTURE_LIGHTING_MODE_HP 0x8167
#endif
#ifndef GL_TEXTURE_POST_SPECULAR_HP
#define GL_TEXTURE_POST_SPECULAR_HP 0x8168
#endif
#ifndef GL_TEXTURE_PRE_SPECULAR_HP
#define GL_TEXTURE_PRE_SPECULAR_HP 0x8169
#endif
#ifndef GL_MAX_ELEMENTS_VERTICES_EXT
#define GL_MAX_ELEMENTS_VERTICES_EXT 0x80E8
#endif
#ifndef GL_MAX_ELEMENTS_INDICES_EXT
#define GL_MAX_ELEMENTS_INDICES_EXT 0x80E9
#endif
#ifndef GL_PHONG_WIN
#define GL_PHONG_WIN 0x80EA
#endif
#ifndef GL_PHONG_HINT_WIN
#define GL_PHONG_HINT_WIN 0x80EB
#endif
#ifndef GL_FOG_SPECULAR_TEXTURE_WIN
#define GL_FOG_SPECULAR_TEXTURE_WIN 0x80EC
#endif
#ifndef GL_FRAGMENT_MATERIAL_EXT
#define GL_FRAGMENT_MATERIAL_EXT 0x8349
#endif
#ifndef GL_FRAGMENT_NORMAL_EXT
#define GL_FRAGMENT_NORMAL_EXT 0x834A
#endif
#ifndef GL_FRAGMENT_COLOR_EXT
#define GL_FRAGMENT_COLOR_EXT 0x834C
#endif
#ifndef GL_ATTENUATION_EXT
#define GL_ATTENUATION_EXT 0x834D
#endif
#ifndef GL_SHADOW_ATTENUATION_EXT
#define GL_SHADOW_ATTENUATION_EXT 0x834E
#endif
#ifndef GL_TEXTURE_APPLICATION_MODE_EXT
#define GL_TEXTURE_APPLICATION_MODE_EXT 0x834F
#endif
#ifndef GL_TEXTURE_LIGHT_EXT
#define GL_TEXTURE_LIGHT_EXT 0x8350
#endif
#ifndef GL_TEXTURE_MATERIAL_FACE_EXT
#define GL_TEXTURE_MATERIAL_FACE_EXT 0x8351
#endif
#ifndef GL_TEXTURE_MATERIAL_PARAMETER_EXT
#define GL_TEXTURE_MATERIAL_PARAMETER_EXT 0x8352
#endif
#ifndef GL_ALPHA_MIN_SGIX
#define GL_ALPHA_MIN_SGIX 0x8320
#endif
#ifndef GL_ALPHA_MAX_SGIX
#define GL_ALPHA_MAX_SGIX 0x8321
#endif
#ifndef GL_PIXEL_TEX_GEN_Q_CEILING_SGIX
#define GL_PIXEL_TEX_GEN_Q_CEILING_SGIX 0x8184
#endif
#ifndef GL_PIXEL_TEX_GEN_Q_ROUND_SGIX
#define GL_PIXEL_TEX_GEN_Q_ROUND_SGIX 0x8185
#endif
#ifndef GL_PIXEL_TEX_GEN_Q_FLOOR_SGIX
#define GL_PIXEL_TEX_GEN_Q_FLOOR_SGIX 0x8186
#endif
#ifndef GL_PIXEL_TEX_GEN_ALPHA_REPLACE_SGIX
#define GL_PIXEL_TEX_GEN_ALPHA_REPLACE_SGIX 0x8187
#endif
#ifndef GL_PIXEL_TEX_GEN_ALPHA_NO_REPLACE_SGIX
#define GL_PIXEL_TEX_GEN_ALPHA_NO_REPLACE_SGIX 0x8188
#endif
#ifndef GL_PIXEL_TEX_GEN_ALPHA_LS_SGIX
#define GL_PIXEL_TEX_GEN_ALPHA_LS_SGIX 0x8189
#endif
#ifndef GL_PIXEL_TEX_GEN_ALPHA_MS_SGIX
#define GL_PIXEL_TEX_GEN_ALPHA_MS_SGIX 0x818A
#endif
#ifndef GL_BGR_EXT
#define GL_BGR_EXT 0x80E0
#endif
#ifndef GL_BGRA_EXT
#define GL_BGRA_EXT 0x80E1
#endif
#ifndef GL_ASYNC_MARKER_SGIX
#define GL_ASYNC_MARKER_SGIX 0x8329
#endif
#ifndef GL_ASYNC_TEX_IMAGE_SGIX
#define GL_ASYNC_TEX_IMAGE_SGIX 0x835C
#endif
#ifndef GL_ASYNC_DRAW_PIXELS_SGIX
#define GL_ASYNC_DRAW_PIXELS_SGIX 0x835D
#endif
#ifndef GL_ASYNC_READ_PIXELS_SGIX
#define GL_ASYNC_READ_PIXELS_SGIX 0x835E
#endif
#ifndef GL_MAX_ASYNC_TEX_IMAGE_SGIX
#define GL_MAX_ASYNC_TEX_IMAGE_SGIX 0x835F
#endif
#ifndef GL_MAX_ASYNC_DRAW_PIXELS_SGIX
#define GL_MAX_ASYNC_DRAW_PIXELS_SGIX 0x8360
#endif
#ifndef GL_MAX_ASYNC_READ_PIXELS_SGIX
#define GL_MAX_ASYNC_READ_PIXELS_SGIX 0x8361
#endif
#ifndef GL_ASYNC_HISTOGRAM_SGIX
#define GL_ASYNC_HISTOGRAM_SGIX 0x832C
#endif
#ifndef GL_MAX_ASYNC_HISTOGRAM_SGIX
#define GL_MAX_ASYNC_HISTOGRAM_SGIX 0x832D
#endif
#ifndef GL_PARALLEL_ARRAYS_INTEL
#define GL_PARALLEL_ARRAYS_INTEL 0x83F4
#endif
#ifndef GL_VERTEX_ARRAY_PARALLEL_POINTERS_INTEL
#define GL_VERTEX_ARRAY_PARALLEL_POINTERS_INTEL 0x83F5
#endif
#ifndef GL_NORMAL_ARRAY_PARALLEL_POINTERS_INTEL
#define GL_NORMAL_ARRAY_PARALLEL_POINTERS_INTEL 0x83F6
#endif
#ifndef GL_COLOR_ARRAY_PARALLEL_POINTERS_INTEL
#define GL_COLOR_ARRAY_PARALLEL_POINTERS_INTEL 0x83F7
#endif
#ifndef GL_TEXTURE_COORD_ARRAY_PARALLEL_POINTERS_INTEL
#define GL_TEXTURE_COORD_ARRAY_PARALLEL_POINTERS_INTEL 0x83F8
#endif
#ifndef GL_OCCLUSION_TEST_HP
#define GL_OCCLUSION_TEST_HP 0x8165
#endif
#ifndef GL_OCCLUSION_TEST_RESULT_HP
#define GL_OCCLUSION_TEST_RESULT_HP 0x8166
#endif
#ifndef GL_PIXEL_TRANSFORM_2D_EXT
#define GL_PIXEL_TRANSFORM_2D_EXT 0x8330
#endif
#ifndef GL_PIXEL_MAG_FILTER_EXT
#define GL_PIXEL_MAG_FILTER_EXT 0x8331
#endif
#ifndef GL_PIXEL_MIN_FILTER_EXT
#define GL_PIXEL_MIN_FILTER_EXT 0x8332
#endif
#ifndef GL_PIXEL_CUBIC_WEIGHT_EXT
#define GL_PIXEL_CUBIC_WEIGHT_EXT 0x8333
#endif
#ifndef GL_CUBIC_EXT
#define GL_CUBIC_EXT 0x8334
#endif
#ifndef GL_AVERAGE_EXT
#define GL_AVERAGE_EXT 0x8335
#endif
#ifndef GL_PIXEL_TRANSFORM_2D_STACK_DEPTH_EXT
#define GL_PIXEL_TRANSFORM_2D_STACK_DEPTH_EXT 0x8336
#endif
#ifndef GL_MAX_PIXEL_TRANSFORM_2D_STACK_DEPTH_EXT
#define GL_MAX_PIXEL_TRANSFORM_2D_STACK_DEPTH_EXT 0x8337
#endif
#ifndef GL_PIXEL_TRANSFORM_2D_MATRIX_EXT
#define GL_PIXEL_TRANSFORM_2D_MATRIX_EXT 0x8338
#endif
#ifndef GL_SHARED_TEXTURE_PALETTE_EXT
#define GL_SHARED_TEXTURE_PALETTE_EXT 0x81FB
#endif
#ifndef GL_LIGHT_MODEL_COLOR_CONTROL_EXT
#define GL_LIGHT_MODEL_COLOR_CONTROL_EXT 0x81F8
#endif
#ifndef GL_SINGLE_COLOR_EXT
#define GL_SINGLE_COLOR_EXT 0x81F9
#endif
#ifndef GL_SEPARATE_SPECULAR_COLOR_EXT
#define GL_SEPARATE_SPECULAR_COLOR_EXT 0x81FA
#endif
#ifndef GL_COLOR_SUM_EXT
#define GL_COLOR_SUM_EXT 0x8458
#endif
#ifndef GL_CURRENT_SECONDARY_COLOR_EXT
#define GL_CURRENT_SECONDARY_COLOR_EXT 0x8459
#endif
#ifndef GL_SECONDARY_COLOR_ARRAY_SIZE_EXT
#define GL_SECONDARY_COLOR_ARRAY_SIZE_EXT 0x845A
#endif
#ifndef GL_SECONDARY_COLOR_ARRAY_TYPE_EXT
#define GL_SECONDARY_COLOR_ARRAY_TYPE_EXT 0x845B
#endif
#ifndef GL_SECONDARY_COLOR_ARRAY_STRIDE_EXT
#define GL_SECONDARY_COLOR_ARRAY_STRIDE_EXT 0x845C
#endif
#ifndef GL_SECONDARY_COLOR_ARRAY_POINTER_EXT
#define GL_SECONDARY_COLOR_ARRAY_POINTER_EXT 0x845D
#endif
#ifndef GL_SECONDARY_COLOR_ARRAY_EXT
#define GL_SECONDARY_COLOR_ARRAY_EXT 0x845E
#endif
#ifndef GL_PERTURB_EXT
#define GL_PERTURB_EXT 0x85AE
#endif
#ifndef GL_TEXTURE_NORMAL_EXT
#define GL_TEXTURE_NORMAL_EXT 0x85AF
#endif
#ifndef GL_FOG_COORDINATE_SOURCE_EXT
#define GL_FOG_COORDINATE_SOURCE_EXT 0x8450
#endif
#ifndef GL_FOG_COORDINATE_EXT
#define GL_FOG_COORDINATE_EXT 0x8451
#endif
#ifndef GL_FRAGMENT_DEPTH_EXT
#define GL_FRAGMENT_DEPTH_EXT 0x8452
#endif
#ifndef GL_CURRENT_FOG_COORDINATE_EXT
#define GL_CURRENT_FOG_COORDINATE_EXT 0x8453
#endif
#ifndef GL_FOG_COORDINATE_ARRAY_TYPE_EXT
#define GL_FOG_COORDINATE_ARRAY_TYPE_EXT 0x8454
#endif
#ifndef GL_FOG_COORDINATE_ARRAY_STRIDE_EXT
#define GL_FOG_COORDINATE_ARRAY_STRIDE_EXT 0x8455
#endif
#ifndef GL_FOG_COORDINATE_ARRAY_POINTER_EXT
#define GL_FOG_COORDINATE_ARRAY_POINTER_EXT 0x8456
#endif
#ifndef GL_FOG_COORDINATE_ARRAY_EXT
#define GL_FOG_COORDINATE_ARRAY_EXT 0x8457
#endif
#ifndef GL_SCREEN_COORDINATES_REND
#define GL_SCREEN_COORDINATES_REND 0x8490
#endif
#ifndef GL_INVERTED_SCREEN_W_REND
#define GL_INVERTED_SCREEN_W_REND 0x8491
#endif
#ifndef GL_TANGENT_ARRAY_EXT
#define GL_TANGENT_ARRAY_EXT 0x8439
#endif
#ifndef GL_BINORMAL_ARRAY_EXT
#define GL_BINORMAL_ARRAY_EXT 0x843A
#endif
#ifndef GL_CURRENT_TANGENT_EXT
#define GL_CURRENT_TANGENT_EXT 0x843B
#endif
#ifndef GL_CURRENT_BINORMAL_EXT
#define GL_CURRENT_BINORMAL_EXT 0x843C
#endif
#ifndef GL_TANGENT_ARRAY_TYPE_EXT
#define GL_TANGENT_ARRAY_TYPE_EXT 0x843E
#endif
#ifndef GL_TANGENT_ARRAY_STRIDE_EXT
#define GL_TANGENT_ARRAY_STRIDE_EXT 0x843F
#endif
#ifndef GL_BINORMAL_ARRAY_TYPE_EXT
#define GL_BINORMAL_ARRAY_TYPE_EXT 0x8440
#endif
#ifndef GL_BINORMAL_ARRAY_STRIDE_EXT
#define GL_BINORMAL_ARRAY_STRIDE_EXT 0x8441
#endif
#ifndef GL_TANGENT_ARRAY_POINTER_EXT
#define GL_TANGENT_ARRAY_POINTER_EXT 0x8442
#endif
#ifndef GL_BINORMAL_ARRAY_POINTER_EXT
#define GL_BINORMAL_ARRAY_POINTER_EXT 0x8443
#endif
#ifndef GL_MAP1_TANGENT_EXT
#define GL_MAP1_TANGENT_EXT 0x8444
#endif
#ifndef GL_MAP2_TANGENT_EXT
#define GL_MAP2_TANGENT_EXT 0x8445
#endif
#ifndef GL_MAP1_BINORMAL_EXT
#define GL_MAP1_BINORMAL_EXT 0x8446
#endif
#ifndef GL_MAP2_BINORMAL_EXT
#define GL_MAP2_BINORMAL_EXT 0x8447
#endif
#ifndef GL_COMBINE_EXT
#define GL_COMBINE_EXT 0x8570
#endif
#ifndef GL_COMBINE_RGB_EXT
#define GL_COMBINE_RGB_EXT 0x8571
#endif
#ifndef GL_COMBINE_ALPHA_EXT
#define GL_COMBINE_ALPHA_EXT 0x8572
#endif
#ifndef GL_RGB_SCALE_EXT
#define GL_RGB_SCALE_EXT 0x8573
#endif
#ifndef GL_ADD_SIGNED_EXT
#define GL_ADD_SIGNED_EXT 0x8574
#endif
#ifndef GL_INTERPOLATE_EXT
#define GL_INTERPOLATE_EXT 0x8575
#endif
#ifndef GL_CONSTANT_EXT
#define GL_CONSTANT_EXT 0x8576
#endif
#ifndef GL_PRIMARY_COLOR_EXT
#define GL_PRIMARY_COLOR_EXT 0x8577
#endif
#ifndef GL_PREVIOUS_EXT
#define GL_PREVIOUS_EXT 0x8578
#endif
#ifndef GL_SOURCE0_RGB_EXT
#define GL_SOURCE0_RGB_EXT 0x8580
#endif
#ifndef GL_SOURCE1_RGB_EXT
#define GL_SOURCE1_RGB_EXT 0x8581
#endif
#ifndef GL_SOURCE2_RGB_EXT
#define GL_SOURCE2_RGB_EXT 0x8582
#endif
#ifndef GL_SOURCE0_ALPHA_EXT
#define GL_SOURCE0_ALPHA_EXT 0x8588
#endif
#ifndef GL_SOURCE1_ALPHA_EXT
#define GL_SOURCE1_ALPHA_EXT 0x8589
#endif
#ifndef GL_SOURCE2_ALPHA_EXT
#define GL_SOURCE2_ALPHA_EXT 0x858A
#endif
#ifndef GL_OPERAND0_RGB_EXT
#define GL_OPERAND0_RGB_EXT 0x8590
#endif
#ifndef GL_OPERAND1_RGB_EXT
#define GL_OPERAND1_RGB_EXT 0x8591
#endif
#ifndef GL_OPERAND2_RGB_EXT
#define GL_OPERAND2_RGB_EXT 0x8592
#endif
#ifndef GL_OPERAND0_ALPHA_EXT
#define GL_OPERAND0_ALPHA_EXT 0x8598
#endif
#ifndef GL_OPERAND1_ALPHA_EXT
#define GL_OPERAND1_ALPHA_EXT 0x8599
#endif
#ifndef GL_OPERAND2_ALPHA_EXT
#define GL_OPERAND2_ALPHA_EXT 0x859A
#endif
#ifndef GL_LIGHT_MODEL_SPECULAR_VECTOR_APPLE
#define GL_LIGHT_MODEL_SPECULAR_VECTOR_APPLE 0x85B0
#endif
#ifndef GL_TRANSFORM_HINT_APPLE
#define GL_TRANSFORM_HINT_APPLE 0x85B1
#endif
#ifndef GL_FOG_SCALE_SGIX
#define GL_FOG_SCALE_SGIX 0x81FC
#endif
#ifndef GL_FOG_SCALE_VALUE_SGIX
#define GL_FOG_SCALE_VALUE_SGIX 0x81FD
#endif
#ifndef GL_UNPACK_CONSTANT_DATA_SUNX
#define GL_UNPACK_CONSTANT_DATA_SUNX 0x81D5
#endif
#ifndef GL_TEXTURE_CONSTANT_DATA_SUNX
#define GL_TEXTURE_CONSTANT_DATA_SUNX 0x81D6
#endif
#ifndef GL_GLOBAL_ALPHA_SUN
#define GL_GLOBAL_ALPHA_SUN 0x81D9
#endif
#ifndef GL_GLOBAL_ALPHA_FACTOR_SUN
#define GL_GLOBAL_ALPHA_FACTOR_SUN 0x81DA
#endif
#ifndef GL_RESTART_SUN
#define GL_RESTART_SUN 0x0001
#endif
#ifndef GL_REPLACE_MIDDLE_SUN
#define GL_REPLACE_MIDDLE_SUN 0x0002
#endif
#ifndef GL_REPLACE_OLDEST_SUN
#define GL_REPLACE_OLDEST_SUN 0x0003
#endif
#ifndef GL_TRIANGLE_LIST_SUN
#define GL_TRIANGLE_LIST_SUN 0x81D7
#endif
#ifndef GL_REPLACEMENT_CODE_SUN
#define GL_REPLACEMENT_CODE_SUN 0x81D8
#endif
#ifndef GL_REPLACEMENT_CODE_ARRAY_SUN
#define GL_REPLACEMENT_CODE_ARRAY_SUN 0x85C0
#endif
#ifndef GL_REPLACEMENT_CODE_ARRAY_TYPE_SUN
#define GL_REPLACEMENT_CODE_ARRAY_TYPE_SUN 0x85C1
#endif
#ifndef GL_REPLACEMENT_CODE_ARRAY_STRIDE_SUN
#define GL_REPLACEMENT_CODE_ARRAY_STRIDE_SUN 0x85C2
#endif
#ifndef GL_REPLACEMENT_CODE_ARRAY_POINTER_SUN
#define GL_REPLACEMENT_CODE_ARRAY_POINTER_SUN 0x85C3
#endif
#ifndef GL_R1UI_V3F_SUN
#define GL_R1UI_V3F_SUN 0x85C4
#endif
#ifndef GL_R1UI_C4UB_V3F_SUN
#define GL_R1UI_C4UB_V3F_SUN 0x85C5
#endif
#ifndef GL_R1UI_C3F_V3F_SUN
#define GL_R1UI_C3F_V3F_SUN 0x85C6
#endif
#ifndef GL_R1UI_N3F_V3F_SUN
#define GL_R1UI_N3F_V3F_SUN 0x85C7
#endif
#ifndef GL_R1UI_C4F_N3F_V3F_SUN
#define GL_R1UI_C4F_N3F_V3F_SUN 0x85C8
#endif
#ifndef GL_R1UI_T2F_V3F_SUN
#define GL_R1UI_T2F_V3F_SUN 0x85C9
#endif
#ifndef GL_R1UI_T2F_N3F_V3F_SUN
#define GL_R1UI_T2F_N3F_V3F_SUN 0x85CA
#endif
#ifndef GL_R1UI_T2F_C4F_N3F_V3F_SUN
#define GL_R1UI_T2F_C4F_N3F_V3F_SUN 0x85CB
#endif
#ifndef GL_BLEND_DST_RGB_EXT
#define GL_BLEND_DST_RGB_EXT 0x80C8
#endif
#ifndef GL_BLEND_SRC_RGB_EXT
#define GL_BLEND_SRC_RGB_EXT 0x80C9
#endif
#ifndef GL_BLEND_DST_ALPHA_EXT
#define GL_BLEND_DST_ALPHA_EXT 0x80CA
#endif
#ifndef GL_BLEND_SRC_ALPHA_EXT
#define GL_BLEND_SRC_ALPHA_EXT 0x80CB
#endif
#ifndef GL_RED_MIN_CLAMP_INGR
#define GL_RED_MIN_CLAMP_INGR 0x8560
#endif
#ifndef GL_GREEN_MIN_CLAMP_INGR
#define GL_GREEN_MIN_CLAMP_INGR 0x8561
#endif
#ifndef GL_BLUE_MIN_CLAMP_INGR
#define GL_BLUE_MIN_CLAMP_INGR 0x8562
#endif
#ifndef GL_ALPHA_MIN_CLAMP_INGR
#define GL_ALPHA_MIN_CLAMP_INGR 0x8563
#endif
#ifndef GL_RED_MAX_CLAMP_INGR
#define GL_RED_MAX_CLAMP_INGR 0x8564
#endif
#ifndef GL_GREEN_MAX_CLAMP_INGR
#define GL_GREEN_MAX_CLAMP_INGR 0x8565
#endif
#ifndef GL_BLUE_MAX_CLAMP_INGR
#define GL_BLUE_MAX_CLAMP_INGR 0x8566
#endif
#ifndef GL_ALPHA_MAX_CLAMP_INGR
#define GL_ALPHA_MAX_CLAMP_INGR 0x8567
#endif
#ifndef GL_INTERLACE_READ_INGR
#define GL_INTERLACE_READ_INGR 0x8568
#endif
#ifndef GL_INCR_WRAP_EXT
#define GL_INCR_WRAP_EXT 0x8507
#endif
#ifndef GL_DECR_WRAP_EXT
#define GL_DECR_WRAP_EXT 0x8508
#endif
#ifndef GL_422_EXT
#define GL_422_EXT 0x80CC
#endif
#ifndef GL_422_REV_EXT
#define GL_422_REV_EXT 0x80CD
#endif
#ifndef GL_422_AVERAGE_EXT
#define GL_422_AVERAGE_EXT 0x80CE
#endif
#ifndef GL_422_REV_AVERAGE_EXT
#define GL_422_REV_AVERAGE_EXT 0x80CF
#endif
#ifndef GL_NORMAL_MAP_NV
#define GL_NORMAL_MAP_NV 0x8511
#endif
#ifndef GL_REFLECTION_MAP_NV
#define GL_REFLECTION_MAP_NV 0x8512
#endif
#ifndef GL_NORMAL_MAP_EXT
#define GL_NORMAL_MAP_EXT 0x8511
#endif
#ifndef GL_REFLECTION_MAP_EXT
#define GL_REFLECTION_MAP_EXT 0x8512
#endif
#ifndef GL_TEXTURE_CUBE_MAP_EXT
#define GL_TEXTURE_CUBE_MAP_EXT 0x8513
#endif
#ifndef GL_TEXTURE_BINDING_CUBE_MAP_EXT
#define GL_TEXTURE_BINDING_CUBE_MAP_EXT 0x8514
#endif
#ifndef GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT 0x8515
#endif
#ifndef GL_TEXTURE_CUBE_MAP_NEGATIVE_X_EXT
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X_EXT 0x8516
#endif
#ifndef GL_TEXTURE_CUBE_MAP_POSITIVE_Y_EXT
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y_EXT 0x8517
#endif
#ifndef GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_EXT
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_EXT 0x8518
#endif
#ifndef GL_TEXTURE_CUBE_MAP_POSITIVE_Z_EXT
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z_EXT 0x8519
#endif
#ifndef GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_EXT
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_EXT 0x851A
#endif
#ifndef GL_PROXY_TEXTURE_CUBE_MAP_EXT
#define GL_PROXY_TEXTURE_CUBE_MAP_EXT 0x851B
#endif
#ifndef GL_MAX_CUBE_MAP_TEXTURE_SIZE_EXT
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE_EXT 0x851C
#endif
#ifndef GL_WRAP_BORDER_SUN
#define GL_WRAP_BORDER_SUN 0x81D4
#endif
#ifndef GL_MAX_TEXTURE_LOD_BIAS_EXT
#define GL_MAX_TEXTURE_LOD_BIAS_EXT 0x84FD
#endif
#ifndef GL_TEXTURE_FILTER_CONTROL_EXT
#define GL_TEXTURE_FILTER_CONTROL_EXT 0x8500
#endif
#ifndef GL_TEXTURE_LOD_BIAS_EXT
#define GL_TEXTURE_LOD_BIAS_EXT 0x8501
#endif
#ifndef GL_TEXTURE_MAX_ANISOTROPY_EXT
#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#endif
#ifndef GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF
#endif
#ifndef GL_MODELVIEW0_STACK_DEPTH_EXT
#define GL_MODELVIEW0_STACK_DEPTH_EXT GL_MODELVIEW_STACK_DEPTH
#endif
#ifndef GL_MODELVIEW1_STACK_DEPTH_EXT
#define GL_MODELVIEW1_STACK_DEPTH_EXT 0x8502
#endif
#ifndef GL_MODELVIEW0_MATRIX_EXT
#define GL_MODELVIEW0_MATRIX_EXT GL_MODELVIEW_MATRIX
#endif
#ifndef GL_MODELVIEW1_MATRIX_EXT
#define GL_MODELVIEW1_MATRIX_EXT 0x8506
#endif
#ifndef GL_VERTEX_WEIGHTING_EXT
#define GL_VERTEX_WEIGHTING_EXT 0x8509
#endif
#ifndef GL_MODELVIEW0_EXT
#define GL_MODELVIEW0_EXT GL_MODELVIEW
#endif
#ifndef GL_MODELVIEW1_EXT
#define GL_MODELVIEW1_EXT 0x850A
#endif
#ifndef GL_CURRENT_VERTEX_WEIGHT_EXT
#define GL_CURRENT_VERTEX_WEIGHT_EXT 0x850B
#endif
#ifndef GL_VERTEX_WEIGHT_ARRAY_EXT
#define GL_VERTEX_WEIGHT_ARRAY_EXT 0x850C
#endif
#ifndef GL_VERTEX_WEIGHT_ARRAY_SIZE_EXT
#define GL_VERTEX_WEIGHT_ARRAY_SIZE_EXT 0x850D
#endif
#ifndef GL_VERTEX_WEIGHT_ARRAY_TYPE_EXT
#define GL_VERTEX_WEIGHT_ARRAY_TYPE_EXT 0x850E
#endif
#ifndef GL_VERTEX_WEIGHT_ARRAY_STRIDE_EXT
#define GL_VERTEX_WEIGHT_ARRAY_STRIDE_EXT 0x850F
#endif
#ifndef GL_VERTEX_WEIGHT_ARRAY_POINTER_EXT
#define GL_VERTEX_WEIGHT_ARRAY_POINTER_EXT 0x8510
#endif
#ifndef GL_MAX_SHININESS_NV
#define GL_MAX_SHININESS_NV 0x8504
#endif
#ifndef GL_MAX_SPOT_EXPONENT_NV
#define GL_MAX_SPOT_EXPONENT_NV 0x8505
#endif
#ifndef GL_VERTEX_ARRAY_RANGE_NV
#define GL_VERTEX_ARRAY_RANGE_NV 0x851D
#endif
#ifndef GL_VERTEX_ARRAY_RANGE_LENGTH_NV
#define GL_VERTEX_ARRAY_RANGE_LENGTH_NV 0x851E
#endif
#ifndef GL_VERTEX_ARRAY_RANGE_VALID_NV
#define GL_VERTEX_ARRAY_RANGE_VALID_NV 0x851F
#endif
#ifndef GL_MAX_VERTEX_ARRAY_RANGE_ELEMENT_NV
#define GL_MAX_VERTEX_ARRAY_RANGE_ELEMENT_NV 0x8520
#endif
#ifndef GL_VERTEX_ARRAY_RANGE_POINTER_NV
#define GL_VERTEX_ARRAY_RANGE_POINTER_NV 0x8521
#endif
#ifndef GL_REGISTER_COMBINERS_NV
#define GL_REGISTER_COMBINERS_NV 0x8522
#endif
#ifndef GL_VARIABLE_A_NV
#define GL_VARIABLE_A_NV 0x8523
#endif
#ifndef GL_VARIABLE_B_NV
#define GL_VARIABLE_B_NV 0x8524
#endif
#ifndef GL_VARIABLE_C_NV
#define GL_VARIABLE_C_NV 0x8525
#endif
#ifndef GL_VARIABLE_D_NV
#define GL_VARIABLE_D_NV 0x8526
#endif
#ifndef GL_VARIABLE_E_NV
#define GL_VARIABLE_E_NV 0x8527
#endif
#ifndef GL_VARIABLE_F_NV
#define GL_VARIABLE_F_NV 0x8528
#endif
#ifndef GL_VARIABLE_G_NV
#define GL_VARIABLE_G_NV 0x8529
#endif
#ifndef GL_CONSTANT_COLOR0_NV
#define GL_CONSTANT_COLOR0_NV 0x852A
#endif
#ifndef GL_CONSTANT_COLOR1_NV
#define GL_CONSTANT_COLOR1_NV 0x852B
#endif
#ifndef GL_PRIMARY_COLOR_NV
#define GL_PRIMARY_COLOR_NV 0x852C
#endif
#ifndef GL_SECONDARY_COLOR_NV
#define GL_SECONDARY_COLOR_NV 0x852D
#endif
#ifndef GL_SPARE0_NV
#define GL_SPARE0_NV 0x852E
#endif
#ifndef GL_SPARE1_NV
#define GL_SPARE1_NV 0x852F
#endif
#ifndef GL_DISCARD_NV
#define GL_DISCARD_NV 0x8530
#endif
#ifndef GL_E_TIMES_F_NV
#define GL_E_TIMES_F_NV 0x8531
#endif
#ifndef GL_SPARE0_PLUS_SECONDARY_COLOR_NV
#define GL_SPARE0_PLUS_SECONDARY_COLOR_NV 0x8532
#endif
#ifndef GL_UNSIGNED_IDENTITY_NV
#define GL_UNSIGNED_IDENTITY_NV 0x8536
#endif
#ifndef GL_UNSIGNED_INVERT_NV
#define GL_UNSIGNED_INVERT_NV 0x8537
#endif
#ifndef GL_EXPAND_NORMAL_NV
#define GL_EXPAND_NORMAL_NV 0x8538
#endif
#ifndef GL_EXPAND_NEGATE_NV
#define GL_EXPAND_NEGATE_NV 0x8539
#endif
#ifndef GL_HALF_BIAS_NORMAL_NV
#define GL_HALF_BIAS_NORMAL_NV 0x853A
#endif
#ifndef GL_HALF_BIAS_NEGATE_NV
#define GL_HALF_BIAS_NEGATE_NV 0x853B
#endif
#ifndef GL_SIGNED_IDENTITY_NV
#define GL_SIGNED_IDENTITY_NV 0x853C
#endif
#ifndef GL_SIGNED_NEGATE_NV
#define GL_SIGNED_NEGATE_NV 0x853D
#endif
#ifndef GL_SCALE_BY_TWO_NV
#define GL_SCALE_BY_TWO_NV 0x853E
#endif
#ifndef GL_SCALE_BY_FOUR_NV
#define GL_SCALE_BY_FOUR_NV 0x853F
#endif
#ifndef GL_SCALE_BY_ONE_HALF_NV
#define GL_SCALE_BY_ONE_HALF_NV 0x8540
#endif
#ifndef GL_BIAS_BY_NEGATIVE_ONE_HALF_NV
#define GL_BIAS_BY_NEGATIVE_ONE_HALF_NV 0x8541
#endif
#ifndef GL_COMBINER_INPUT_NV
#define GL_COMBINER_INPUT_NV 0x8542
#endif
#ifndef GL_COMBINER_MAPPING_NV
#define GL_COMBINER_MAPPING_NV 0x8543
#endif
#ifndef GL_COMBINER_COMPONENT_USAGE_NV
#define GL_COMBINER_COMPONENT_USAGE_NV 0x8544
#endif
#ifndef GL_COMBINER_AB_DOT_PRODUCT_NV
#define GL_COMBINER_AB_DOT_PRODUCT_NV 0x8545
#endif
#ifndef GL_COMBINER_CD_DOT_PRODUCT_NV
#define GL_COMBINER_CD_DOT_PRODUCT_NV 0x8546
#endif
#ifndef GL_COMBINER_MUX_SUM_NV
#define GL_COMBINER_MUX_SUM_NV 0x8547
#endif
#ifndef GL_COMBINER_SCALE_NV
#define GL_COMBINER_SCALE_NV 0x8548
#endif
#ifndef GL_COMBINER_BIAS_NV
#define GL_COMBINER_BIAS_NV 0x8549
#endif
#ifndef GL_COMBINER_AB_OUTPUT_NV
#define GL_COMBINER_AB_OUTPUT_NV 0x854A
#endif
#ifndef GL_COMBINER_CD_OUTPUT_NV
#define GL_COMBINER_CD_OUTPUT_NV 0x854B
#endif
#ifndef GL_COMBINER_SUM_OUTPUT_NV
#define GL_COMBINER_SUM_OUTPUT_NV 0x854C
#endif
#ifndef GL_MAX_GENERAL_COMBINERS_NV
#define GL_MAX_GENERAL_COMBINERS_NV 0x854D
#endif
#ifndef GL_NUM_GENERAL_COMBINERS_NV
#define GL_NUM_GENERAL_COMBINERS_NV 0x854E
#endif
#ifndef GL_COLOR_SUM_CLAMP_NV
#define GL_COLOR_SUM_CLAMP_NV 0x854F
#endif
#ifndef GL_COMBINER0_NV
#define GL_COMBINER0_NV 0x8550
#endif
#ifndef GL_COMBINER1_NV
#define GL_COMBINER1_NV 0x8551
#endif
#ifndef GL_COMBINER2_NV
#define GL_COMBINER2_NV 0x8552
#endif
#ifndef GL_COMBINER3_NV
#define GL_COMBINER3_NV 0x8553
#endif
#ifndef GL_COMBINER4_NV
#define GL_COMBINER4_NV 0x8554
#endif
#ifndef GL_COMBINER5_NV
#define GL_COMBINER5_NV 0x8555
#endif
#ifndef GL_COMBINER6_NV
#define GL_COMBINER6_NV 0x8556
#endif
#ifndef GL_COMBINER7_NV
#define GL_COMBINER7_NV 0x8557
#endif
#ifndef GL_FOG_DISTANCE_MODE_NV
#define GL_FOG_DISTANCE_MODE_NV 0x855A
#endif
#ifndef GL_EYE_RADIAL_NV
#define GL_EYE_RADIAL_NV 0x855B
#endif
#ifndef GL_EYE_PLANE_ABSOLUTE_NV
#define GL_EYE_PLANE_ABSOLUTE_NV 0x855C
#endif
#ifndef GL_EMBOSS_LIGHT_NV
#define GL_EMBOSS_LIGHT_NV 0x855D
#endif
#ifndef GL_EMBOSS_CONSTANT_NV
#define GL_EMBOSS_CONSTANT_NV 0x855E
#endif
#ifndef GL_EMBOSS_MAP_NV
#define GL_EMBOSS_MAP_NV 0x855F
#endif
#ifndef GL_COMBINE4_NV
#define GL_COMBINE4_NV 0x8503
#endif
#ifndef GL_SOURCE3_RGB_NV
#define GL_SOURCE3_RGB_NV 0x8583
#endif
#ifndef GL_SOURCE3_ALPHA_NV
#define GL_SOURCE3_ALPHA_NV 0x858B
#endif
#ifndef GL_OPERAND3_RGB_NV
#define GL_OPERAND3_RGB_NV 0x8593
#endif
#ifndef GL_OPERAND3_ALPHA_NV
#define GL_OPERAND3_ALPHA_NV 0x859B
#endif
#ifndef GL_COMPRESSED_RGB_S3TC_DXT1_EXT
#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT 0x83F0
#endif
#ifndef GL_COMPRESSED_RGBA_S3TC_DXT1_EXT
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT 0x83F1
#endif
#ifndef GL_COMPRESSED_RGBA_S3TC_DXT3_EXT
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT 0x83F2
#endif
#ifndef GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT 0x83F3
#endif
#ifndef GL_CULL_VERTEX_IBM
#define GL_CULL_VERTEX_IBM 103050
#endif
#ifndef GL_VERTEX_ARRAY_LIST_IBM
#define GL_VERTEX_ARRAY_LIST_IBM 103070
#endif
#ifndef GL_NORMAL_ARRAY_LIST_IBM
#define GL_NORMAL_ARRAY_LIST_IBM 103071
#endif
#ifndef GL_COLOR_ARRAY_LIST_IBM
#define GL_COLOR_ARRAY_LIST_IBM 103072
#endif
#ifndef GL_INDEX_ARRAY_LIST_IBM
#define GL_INDEX_ARRAY_LIST_IBM 103073
#endif
#ifndef GL_TEXTURE_COORD_ARRAY_LIST_IBM
#define GL_TEXTURE_COORD_ARRAY_LIST_IBM 103074
#endif
#ifndef GL_EDGE_FLAG_ARRAY_LIST_IBM
#define GL_EDGE_FLAG_ARRAY_LIST_IBM 103075
#endif
#ifndef GL_FOG_COORDINATE_ARRAY_LIST_IBM
#define GL_FOG_COORDINATE_ARRAY_LIST_IBM 103076
#endif
#ifndef GL_SECONDARY_COLOR_ARRAY_LIST_IBM
#define GL_SECONDARY_COLOR_ARRAY_LIST_IBM 103077
#endif
#ifndef GL_VERTEX_ARRAY_LIST_STRIDE_IBM
#define GL_VERTEX_ARRAY_LIST_STRIDE_IBM 103080
#endif
#ifndef GL_NORMAL_ARRAY_LIST_STRIDE_IBM
#define GL_NORMAL_ARRAY_LIST_STRIDE_IBM 103081
#endif
#ifndef GL_COLOR_ARRAY_LIST_STRIDE_IBM
#define GL_COLOR_ARRAY_LIST_STRIDE_IBM 103082
#endif
#ifndef GL_INDEX_ARRAY_LIST_STRIDE_IBM
#define GL_INDEX_ARRAY_LIST_STRIDE_IBM 103083
#endif
#ifndef GL_TEXTURE_COORD_ARRAY_LIST_STRIDE_IBM
#define GL_TEXTURE_COORD_ARRAY_LIST_STRIDE_IBM 103084
#endif
#ifndef GL_EDGE_FLAG_ARRAY_LIST_STRIDE_IBM
#define GL_EDGE_FLAG_ARRAY_LIST_STRIDE_IBM 103085
#endif
#ifndef GL_FOG_COORDINATE_ARRAY_LIST_STRIDE_IBM
#define GL_FOG_COORDINATE_ARRAY_LIST_STRIDE_IBM 103086
#endif
#ifndef GL_SECONDARY_COLOR_ARRAY_LIST_STRIDE_IBM
#define GL_SECONDARY_COLOR_ARRAY_LIST_STRIDE_IBM 103087
#endif
#ifndef GL_PACK_SUBSAMPLE_RATE_SGIX
#define GL_PACK_SUBSAMPLE_RATE_SGIX 0x85A0
#endif
#ifndef GL_UNPACK_SUBSAMPLE_RATE_SGIX
#define GL_UNPACK_SUBSAMPLE_RATE_SGIX 0x85A1
#endif
#ifndef GL_PIXEL_SUBSAMPLE_4444_SGIX
#define GL_PIXEL_SUBSAMPLE_4444_SGIX 0x85A2
#endif
#ifndef GL_PIXEL_SUBSAMPLE_2424_SGIX
#define GL_PIXEL_SUBSAMPLE_2424_SGIX 0x85A3
#endif
#ifndef GL_PIXEL_SUBSAMPLE_4242_SGIX
#define GL_PIXEL_SUBSAMPLE_4242_SGIX 0x85A4
#endif
#ifndef GL_PACK_SUBSAMPLE_RATE_SGIX
#define GL_PACK_SUBSAMPLE_RATE_SGIX 0x85A0
#endif
#ifndef GL_UNPACK_SUBSAMPLE_RATE_SGIX
#define GL_UNPACK_SUBSAMPLE_RATE_SGIX 0x85A1
#endif
#ifndef GL_PIXEL_SUBSAMPLE_4444_SGIX
#define GL_PIXEL_SUBSAMPLE_4444_SGIX 0x85A2
#endif
#ifndef GL_PIXEL_SUBSAMPLE_2424_SGIX
#define GL_PIXEL_SUBSAMPLE_2424_SGIX 0x85A3
#endif
#ifndef GL_PIXEL_SUBSAMPLE_4242_SGIX
#define GL_PIXEL_SUBSAMPLE_4242_SGIX 0x85A4
#endif
#ifndef GL_YCRCB_SGIX
#define GL_YCRCB_SGIX 0x8318
#endif
#ifndef GL_YCRCBA_SGIX
#define GL_YCRCBA_SGIX 0x8319
#endif
#ifndef GL_DEPTH_PASS_INSTRUMENT_SGIX
#define GL_DEPTH_PASS_INSTRUMENT_SGIX 0x8310
#endif
#ifndef GL_DEPTH_PASS_INSTRUMENT_COUNTERS_SGIX
#define GL_DEPTH_PASS_INSTRUMENT_COUNTERS_SGIX 0x8311
#endif
#ifndef GL_DEPTH_PASS_INSTRUMENT_MAX_SGIX
#define GL_DEPTH_PASS_INSTRUMENT_MAX_SGIX 0x8312
#endif
#ifndef GL_COMPRESSED_RGB_FXT1_3DFX
#define GL_COMPRESSED_RGB_FXT1_3DFX 0x86B0
#endif
#ifndef GL_COMPRESSED_RGBA_FXT1_3DFX
#define GL_COMPRESSED_RGBA_FXT1_3DFX 0x86B1
#endif
#ifndef GL_MULTISAMPLE_3DFX
#define GL_MULTISAMPLE_3DFX 0x86B2
#endif
#ifndef GL_SAMPLE_BUFFERS_3DFX
#define GL_SAMPLE_BUFFERS_3DFX 0x86B3
#endif
#ifndef GL_SAMPLES_3DFX
#define GL_SAMPLES_3DFX 0x86B4
#endif
#ifndef GL_MULTISAMPLE_BIT_3DFX
#define GL_MULTISAMPLE_BIT_3DFX 0x20000000
#endif
#ifndef GL_MULTISAMPLE_EXT
#define GL_MULTISAMPLE_EXT 0x809D
#endif
#ifndef GL_SAMPLE_ALPHA_TO_MASK_EXT
#define GL_SAMPLE_ALPHA_TO_MASK_EXT 0x809E
#endif
#ifndef GL_SAMPLE_ALPHA_TO_ONE_EXT
#define GL_SAMPLE_ALPHA_TO_ONE_EXT 0x809F
#endif
#ifndef GL_SAMPLE_MASK_EXT
#define GL_SAMPLE_MASK_EXT 0x80A0
#endif
#ifndef GL_1PASS_EXT
#define GL_1PASS_EXT 0x80A1
#endif
#ifndef GL_2PASS_0_EXT
#define GL_2PASS_0_EXT 0x80A2
#endif
#ifndef GL_2PASS_1_EXT
#define GL_2PASS_1_EXT 0x80A3
#endif
#ifndef GL_4PASS_0_EXT
#define GL_4PASS_0_EXT 0x80A4
#endif
#ifndef GL_4PASS_1_EXT
#define GL_4PASS_1_EXT 0x80A5
#endif
#ifndef GL_4PASS_2_EXT
#define GL_4PASS_2_EXT 0x80A6
#endif
#ifndef GL_4PASS_3_EXT
#define GL_4PASS_3_EXT 0x80A7
#endif
#ifndef GL_SAMPLE_BUFFERS_EXT
#define GL_SAMPLE_BUFFERS_EXT 0x80A8
#endif
#ifndef GL_SAMPLES_EXT
#define GL_SAMPLES_EXT 0x80A9
#endif
#ifndef GL_SAMPLE_MASK_VALUE_EXT
#define GL_SAMPLE_MASK_VALUE_EXT 0x80AA
#endif
#ifndef GL_SAMPLE_MASK_INVERT_EXT
#define GL_SAMPLE_MASK_INVERT_EXT 0x80AB
#endif
#ifndef GL_SAMPLE_PATTERN_EXT
#define GL_SAMPLE_PATTERN_EXT 0x80AC
#endif
#ifndef GL_MULTISAMPLE_BIT_EXT
#define GL_MULTISAMPLE_BIT_EXT 0x20000000
#endif
#ifndef GL_VERTEX_PRECLIP_SGIX
#define GL_VERTEX_PRECLIP_SGIX 0x83EE
#endif
#ifndef GL_VERTEX_PRECLIP_HINT_SGIX
#define GL_VERTEX_PRECLIP_HINT_SGIX 0x83EF
#endif
#ifndef GL_CONVOLUTION_HINT_SGIX
#define GL_CONVOLUTION_HINT_SGIX 0x8316
#endif
#ifndef GL_PACK_RESAMPLE_SGIX
#define GL_PACK_RESAMPLE_SGIX 0x842C
#endif
#ifndef GL_UNPACK_RESAMPLE_SGIX
#define GL_UNPACK_RESAMPLE_SGIX 0x842D
#endif
#ifndef GL_RESAMPLE_REPLICATE_SGIX
#define GL_RESAMPLE_REPLICATE_SGIX 0x842E
#endif
#ifndef GL_RESAMPLE_ZERO_FILL_SGIX
#define GL_RESAMPLE_ZERO_FILL_SGIX 0x842F
#endif
#ifndef GL_RESAMPLE_DECIMATE_SGIX
#define GL_RESAMPLE_DECIMATE_SGIX 0x8430
#endif
#ifndef GL_EYE_DISTANCE_TO_POINT_SGIS
#define GL_EYE_DISTANCE_TO_POINT_SGIS 0x81F0
#endif
#ifndef GL_OBJECT_DISTANCE_TO_POINT_SGIS
#define GL_OBJECT_DISTANCE_TO_POINT_SGIS 0x81F1
#endif
#ifndef GL_EYE_DISTANCE_TO_LINE_SGIS
#define GL_EYE_DISTANCE_TO_LINE_SGIS 0x81F2
#endif
#ifndef GL_OBJECT_DISTANCE_TO_LINE_SGIS
#define GL_OBJECT_DISTANCE_TO_LINE_SGIS 0x81F3
#endif
#ifndef GL_EYE_POINT_SGIS
#define GL_EYE_POINT_SGIS 0x81F4
#endif
#ifndef GL_OBJECT_POINT_SGIS
#define GL_OBJECT_POINT_SGIS 0x81F5
#endif
#ifndef GL_EYE_LINE_SGIS
#define GL_EYE_LINE_SGIS 0x81F6
#endif
#ifndef GL_OBJECT_LINE_SGIS
#define GL_OBJECT_LINE_SGIS 0x81F7
#endif
#ifndef GL_TEXTURE_COLOR_WRITEMASK_SGIS
#define GL_TEXTURE_COLOR_WRITEMASK_SGIS 0x81EF
#endif
#ifndef GL_DOT3_RGB_EXT
#define GL_DOT3_RGB_EXT 0x8740
#endif
#ifndef GL_DOT3_RGBA_EXT
#define GL_DOT3_RGBA_EXT 0x8741
#endif
#ifndef GL_MIRROR_CLAMP_ATI
#define GL_MIRROR_CLAMP_ATI 0x8742
#endif
#ifndef GL_MIRROR_CLAMP_TO_EDGE_ATI
#define GL_MIRROR_CLAMP_TO_EDGE_ATI 0x8743
#endif
#ifndef GL_ALL_COMPLETED_NV
#define GL_ALL_COMPLETED_NV 0x84F2
#endif
#ifndef GL_FENCE_STATUS_NV
#define GL_FENCE_STATUS_NV 0x84F3
#endif
#ifndef GL_FENCE_CONDITION_NV
#define GL_FENCE_CONDITION_NV 0x84F4
#endif
#ifndef GL_MIRRORED_REPEAT_IBM
#define GL_MIRRORED_REPEAT_IBM 0x8370
#endif
#ifndef GL_EVAL_2D_NV
#define GL_EVAL_2D_NV 0x86C0
#endif
#ifndef GL_EVAL_TRIANGULAR_2D_NV
#define GL_EVAL_TRIANGULAR_2D_NV 0x86C1
#endif
#ifndef GL_MAP_TESSELLATION_NV
#define GL_MAP_TESSELLATION_NV 0x86C2
#endif
#ifndef GL_MAP_ATTRIB_U_ORDER_NV
#define GL_MAP_ATTRIB_U_ORDER_NV 0x86C3
#endif
#ifndef GL_MAP_ATTRIB_V_ORDER_NV
#define GL_MAP_ATTRIB_V_ORDER_NV 0x86C4
#endif
#ifndef GL_EVAL_FRACTIONAL_TESSELLATION_NV
#define GL_EVAL_FRACTIONAL_TESSELLATION_NV 0x86C5
#endif
#ifndef GL_EVAL_VERTEX_ATTRIB0_NV
#define GL_EVAL_VERTEX_ATTRIB0_NV 0x86C6
#endif
#ifndef GL_EVAL_VERTEX_ATTRIB1_NV
#define GL_EVAL_VERTEX_ATTRIB1_NV 0x86C7
#endif
#ifndef GL_EVAL_VERTEX_ATTRIB2_NV
#define GL_EVAL_VERTEX_ATTRIB2_NV 0x86C8
#endif
#ifndef GL_EVAL_VERTEX_ATTRIB3_NV
#define GL_EVAL_VERTEX_ATTRIB3_NV 0x86C9
#endif
#ifndef GL_EVAL_VERTEX_ATTRIB4_NV
#define GL_EVAL_VERTEX_ATTRIB4_NV 0x86CA
#endif
#ifndef GL_EVAL_VERTEX_ATTRIB5_NV
#define GL_EVAL_VERTEX_ATTRIB5_NV 0x86CB
#endif
#ifndef GL_EVAL_VERTEX_ATTRIB6_NV
#define GL_EVAL_VERTEX_ATTRIB6_NV 0x86CC
#endif
#ifndef GL_EVAL_VERTEX_ATTRIB7_NV
#define GL_EVAL_VERTEX_ATTRIB7_NV 0x86CD
#endif
#ifndef GL_EVAL_VERTEX_ATTRIB8_NV
#define GL_EVAL_VERTEX_ATTRIB8_NV 0x86CE
#endif
#ifndef GL_EVAL_VERTEX_ATTRIB9_NV
#define GL_EVAL_VERTEX_ATTRIB9_NV 0x86CF
#endif
#ifndef GL_EVAL_VERTEX_ATTRIB10_NV
#define GL_EVAL_VERTEX_ATTRIB10_NV 0x86D0
#endif
#ifndef GL_EVAL_VERTEX_ATTRIB11_NV
#define GL_EVAL_VERTEX_ATTRIB11_NV 0x86D1
#endif
#ifndef GL_EVAL_VERTEX_ATTRIB12_NV
#define GL_EVAL_VERTEX_ATTRIB12_NV 0x86D2
#endif
#ifndef GL_EVAL_VERTEX_ATTRIB13_NV
#define GL_EVAL_VERTEX_ATTRIB13_NV 0x86D3
#endif
#ifndef GL_EVAL_VERTEX_ATTRIB14_NV
#define GL_EVAL_VERTEX_ATTRIB14_NV 0x86D4
#endif
#ifndef GL_EVAL_VERTEX_ATTRIB15_NV
#define GL_EVAL_VERTEX_ATTRIB15_NV 0x86D5
#endif
#ifndef GL_MAX_MAP_TESSELLATION_NV
#define GL_MAX_MAP_TESSELLATION_NV 0x86D6
#endif
#ifndef GL_MAX_RATIONAL_EVAL_ORDER_NV
#define GL_MAX_RATIONAL_EVAL_ORDER_NV 0x86D7
#endif
#ifndef GL_DEPTH_STENCIL_NV
#define GL_DEPTH_STENCIL_NV 0x84F9
#endif
#ifndef GL_UNSIGNED_INT_24_8_NV
#define GL_UNSIGNED_INT_24_8_NV 0x84FA
#endif
#ifndef GL_PER_STAGE_CONSTANTS_NV
#define GL_PER_STAGE_CONSTANTS_NV 0x8535
#endif
#ifndef GL_TEXTURE_RECTANGLE_NV
#define GL_TEXTURE_RECTANGLE_NV 0x84F5
#endif
#ifndef GL_TEXTURE_BINDING_RECTANGLE_NV
#define GL_TEXTURE_BINDING_RECTANGLE_NV 0x84F6
#endif
#ifndef GL_PROXY_TEXTURE_RECTANGLE_NV
#define GL_PROXY_TEXTURE_RECTANGLE_NV 0x84F7
#endif
#ifndef GL_MAX_RECTANGLE_TEXTURE_SIZE_NV
#define GL_MAX_RECTANGLE_TEXTURE_SIZE_NV 0x84F8
#endif
#ifndef GL_OFFSET_TEXTURE_RECTANGLE_NV
#define GL_OFFSET_TEXTURE_RECTANGLE_NV 0x864C
#endif
#ifndef GL_OFFSET_TEXTURE_RECTANGLE_SCALE_NV
#define GL_OFFSET_TEXTURE_RECTANGLE_SCALE_NV 0x864D
#endif
#ifndef GL_DOT_PRODUCT_TEXTURE_RECTANGLE_NV
#define GL_DOT_PRODUCT_TEXTURE_RECTANGLE_NV 0x864E
#endif
#ifndef GL_RGBA_UNSIGNED_DOT_PRODUCT_MAPPING_NV
#define GL_RGBA_UNSIGNED_DOT_PRODUCT_MAPPING_NV 0x86D9
#endif
#ifndef GL_UNSIGNED_INT_S8_S8_8_8_NV
#define GL_UNSIGNED_INT_S8_S8_8_8_NV 0x86DA
#endif
#ifndef GL_UNSIGNED_INT_8_8_S8_S8_REV_NV
#define GL_UNSIGNED_INT_8_8_S8_S8_REV_NV 0x86DB
#endif
#ifndef GL_DSDT_MAG_INTENSITY_NV
#define GL_DSDT_MAG_INTENSITY_NV 0x86DC
#endif
#ifndef GL_SHADER_CONSISTENT_NV
#define GL_SHADER_CONSISTENT_NV 0x86DD
#endif
#ifndef GL_TEXTURE_SHADER_NV
#define GL_TEXTURE_SHADER_NV 0x86DE
#endif
#ifndef GL_SHADER_OPERATION_NV
#define GL_SHADER_OPERATION_NV 0x86DF
#endif
#ifndef GL_CULL_MODES_NV
#define GL_CULL_MODES_NV 0x86E0
#endif
#ifndef GL_OFFSET_TEXTURE_MATRIX_NV
#define GL_OFFSET_TEXTURE_MATRIX_NV 0x86E1
#endif
#ifndef GL_OFFSET_TEXTURE_SCALE_NV
#define GL_OFFSET_TEXTURE_SCALE_NV 0x86E2
#endif
#ifndef GL_OFFSET_TEXTURE_BIAS_NV
#define GL_OFFSET_TEXTURE_BIAS_NV 0x86E3
#endif
#ifndef GL_OFFSET_TEXTURE_2D_MATRIX_NV
#define GL_OFFSET_TEXTURE_2D_MATRIX_NV GL_OFFSET_TEXTURE_MATRIX_NV
#endif
#ifndef GL_OFFSET_TEXTURE_2D_SCALE_NV
#define GL_OFFSET_TEXTURE_2D_SCALE_NV GL_OFFSET_TEXTURE_SCALE_NV
#endif
#ifndef GL_OFFSET_TEXTURE_2D_BIAS_NV
#define GL_OFFSET_TEXTURE_2D_BIAS_NV GL_OFFSET_TEXTURE_BIAS_NV
#endif
#ifndef GL_PREVIOUS_TEXTURE_INPUT_NV
#define GL_PREVIOUS_TEXTURE_INPUT_NV 0x86E4
#endif
#ifndef GL_CONST_EYE_NV
#define GL_CONST_EYE_NV 0x86E5
#endif
#ifndef GL_PASS_THROUGH_NV
#define GL_PASS_THROUGH_NV 0x86E6
#endif
#ifndef GL_CULL_FRAGMENT_NV
#define GL_CULL_FRAGMENT_NV 0x86E7
#endif
#ifndef GL_OFFSET_TEXTURE_2D_NV
#define GL_OFFSET_TEXTURE_2D_NV 0x86E8
#endif
#ifndef GL_DEPENDENT_AR_TEXTURE_2D_NV
#define GL_DEPENDENT_AR_TEXTURE_2D_NV 0x86E9
#endif
#ifndef GL_DEPENDENT_GB_TEXTURE_2D_NV
#define GL_DEPENDENT_GB_TEXTURE_2D_NV 0x86EA
#endif
#ifndef GL_DOT_PRODUCT_NV
#define GL_DOT_PRODUCT_NV 0x86EC
#endif
#ifndef GL_DOT_PRODUCT_DEPTH_REPLACE_NV
#define GL_DOT_PRODUCT_DEPTH_REPLACE_NV 0x86ED
#endif
#ifndef GL_DOT_PRODUCT_TEXTURE_2D_NV
#define GL_DOT_PRODUCT_TEXTURE_2D_NV 0x86EE
#endif
#ifndef GL_DOT_PRODUCT_TEXTURE_CUBE_MAP_NV
#define GL_DOT_PRODUCT_TEXTURE_CUBE_MAP_NV 0x86F0
#endif
#ifndef GL_DOT_PRODUCT_DIFFUSE_CUBE_MAP_NV
#define GL_DOT_PRODUCT_DIFFUSE_CUBE_MAP_NV 0x86F1
#endif
#ifndef GL_DOT_PRODUCT_REFLECT_CUBE_MAP_NV
#define GL_DOT_PRODUCT_REFLECT_CUBE_MAP_NV 0x86F2
#endif
#ifndef GL_DOT_PRODUCT_CONST_EYE_REFLECT_CUBE_MAP_NV
#define GL_DOT_PRODUCT_CONST_EYE_REFLECT_CUBE_MAP_NV 0x86F3
#endif
#ifndef GL_HILO_NV
#define GL_HILO_NV 0x86F4
#endif
#ifndef GL_DSDT_NV
#define GL_DSDT_NV 0x86F5
#endif
#ifndef GL_DSDT_MAG_NV
#define GL_DSDT_MAG_NV 0x86F6
#endif
#ifndef GL_DSDT_MAG_VIB_NV
#define GL_DSDT_MAG_VIB_NV 0x86F7
#endif
#ifndef GL_HILO16_NV
#define GL_HILO16_NV 0x86F8
#endif
#ifndef GL_SIGNED_HILO_NV
#define GL_SIGNED_HILO_NV 0x86F9
#endif
#ifndef GL_SIGNED_HILO16_NV
#define GL_SIGNED_HILO16_NV 0x86FA
#endif
#ifndef GL_SIGNED_RGBA_NV
#define GL_SIGNED_RGBA_NV 0x86FB
#endif
#ifndef GL_SIGNED_RGBA8_NV
#define GL_SIGNED_RGBA8_NV 0x86FC
#endif
#ifndef GL_SIGNED_RGB_NV
#define GL_SIGNED_RGB_NV 0x86FE
#endif
#ifndef GL_SIGNED_RGB8_NV
#define GL_SIGNED_RGB8_NV 0x86FF
#endif
#ifndef GL_SIGNED_LUMINANCE_NV
#define GL_SIGNED_LUMINANCE_NV 0x8701
#endif
#ifndef GL_SIGNED_LUMINANCE8_NV
#define GL_SIGNED_LUMINANCE8_NV 0x8702
#endif
#ifndef GL_SIGNED_LUMINANCE_ALPHA_NV
#define GL_SIGNED_LUMINANCE_ALPHA_NV 0x8703
#endif
#ifndef GL_SIGNED_LUMINANCE8_ALPHA8_NV
#define GL_SIGNED_LUMINANCE8_ALPHA8_NV 0x8704
#endif
#ifndef GL_SIGNED_ALPHA_NV
#define GL_SIGNED_ALPHA_NV 0x8705
#endif
#ifndef GL_SIGNED_ALPHA8_NV
#define GL_SIGNED_ALPHA8_NV 0x8706
#endif
#ifndef GL_SIGNED_INTENSITY_NV
#define GL_SIGNED_INTENSITY_NV 0x8707
#endif
#ifndef GL_SIGNED_INTENSITY8_NV
#define GL_SIGNED_INTENSITY8_NV 0x8708
#endif
#ifndef GL_DSDT8_NV
#define GL_DSDT8_NV 0x8709
#endif
#ifndef GL_DSDT8_MAG8_NV
#define GL_DSDT8_MAG8_NV 0x870A
#endif
#ifndef GL_DSDT8_MAG8_INTENSITY8_NV
#define GL_DSDT8_MAG8_INTENSITY8_NV 0x870B
#endif
#ifndef GL_SIGNED_RGB_UNSIGNED_ALPHA_NV
#define GL_SIGNED_RGB_UNSIGNED_ALPHA_NV 0x870C
#endif
#ifndef GL_SIGNED_RGB8_UNSIGNED_ALPHA8_NV
#define GL_SIGNED_RGB8_UNSIGNED_ALPHA8_NV 0x870D
#endif
#ifndef GL_HI_SCALE_NV
#define GL_HI_SCALE_NV 0x870E
#endif
#ifndef GL_LO_SCALE_NV
#define GL_LO_SCALE_NV 0x870F
#endif
#ifndef GL_DS_SCALE_NV
#define GL_DS_SCALE_NV 0x8710
#endif
#ifndef GL_DT_SCALE_NV
#define GL_DT_SCALE_NV 0x8711
#endif
#ifndef GL_MAGNITUDE_SCALE_NV
#define GL_MAGNITUDE_SCALE_NV 0x8712
#endif
#ifndef GL_VIBRANCE_SCALE_NV
#define GL_VIBRANCE_SCALE_NV 0x8713
#endif
#ifndef GL_HI_BIAS_NV
#define GL_HI_BIAS_NV 0x8714
#endif
#ifndef GL_LO_BIAS_NV
#define GL_LO_BIAS_NV 0x8715
#endif
#ifndef GL_DS_BIAS_NV
#define GL_DS_BIAS_NV 0x8716
#endif
#ifndef GL_DT_BIAS_NV
#define GL_DT_BIAS_NV 0x8717
#endif
#ifndef GL_MAGNITUDE_BIAS_NV
#define GL_MAGNITUDE_BIAS_NV 0x8718
#endif
#ifndef GL_VIBRANCE_BIAS_NV
#define GL_VIBRANCE_BIAS_NV 0x8719
#endif
#ifndef GL_TEXTURE_BORDER_VALUES_NV
#define GL_TEXTURE_BORDER_VALUES_NV 0x871A
#endif
#ifndef GL_TEXTURE_HI_SIZE_NV
#define GL_TEXTURE_HI_SIZE_NV 0x871B
#endif
#ifndef GL_TEXTURE_LO_SIZE_NV
#define GL_TEXTURE_LO_SIZE_NV 0x871C
#endif
#ifndef GL_TEXTURE_DS_SIZE_NV
#define GL_TEXTURE_DS_SIZE_NV 0x871D
#endif
#ifndef GL_TEXTURE_DT_SIZE_NV
#define GL_TEXTURE_DT_SIZE_NV 0x871E
#endif
#ifndef GL_TEXTURE_MAG_SIZE_NV
#define GL_TEXTURE_MAG_SIZE_NV 0x871F
#endif
#ifndef GL_DOT_PRODUCT_TEXTURE_3D_NV
#define GL_DOT_PRODUCT_TEXTURE_3D_NV 0x86EF
#endif
#ifndef GL_VERTEX_ARRAY_RANGE_WITHOUT_FLUSH_NV
#define GL_VERTEX_ARRAY_RANGE_WITHOUT_FLUSH_NV 0x8533
#endif
#ifndef GL_VERTEX_PROGRAM_NV
#define GL_VERTEX_PROGRAM_NV 0x8620
#endif
#ifndef GL_VERTEX_STATE_PROGRAM_NV
#define GL_VERTEX_STATE_PROGRAM_NV 0x8621
#endif
#ifndef GL_ATTRIB_ARRAY_SIZE_NV
#define GL_ATTRIB_ARRAY_SIZE_NV 0x8623
#endif
#ifndef GL_ATTRIB_ARRAY_STRIDE_NV
#define GL_ATTRIB_ARRAY_STRIDE_NV 0x8624
#endif
#ifndef GL_ATTRIB_ARRAY_TYPE_NV
#define GL_ATTRIB_ARRAY_TYPE_NV 0x8625
#endif
#ifndef GL_CURRENT_ATTRIB_NV
#define GL_CURRENT_ATTRIB_NV 0x8626
#endif
#ifndef GL_PROGRAM_LENGTH_NV
#define GL_PROGRAM_LENGTH_NV 0x8627
#endif
#ifndef GL_PROGRAM_STRING_NV
#define GL_PROGRAM_STRING_NV 0x8628
#endif
#ifndef GL_MODELVIEW_PROJECTION_NV
#define GL_MODELVIEW_PROJECTION_NV 0x8629
#endif
#ifndef GL_IDENTITY_NV
#define GL_IDENTITY_NV 0x862A
#endif
#ifndef GL_INVERSE_NV
#define GL_INVERSE_NV 0x862B
#endif
#ifndef GL_TRANSPOSE_NV
#define GL_TRANSPOSE_NV 0x862C
#endif
#ifndef GL_INVERSE_TRANSPOSE_NV
#define GL_INVERSE_TRANSPOSE_NV 0x862D
#endif
#ifndef GL_MAX_TRACK_MATRIX_STACK_DEPTH_NV
#define GL_MAX_TRACK_MATRIX_STACK_DEPTH_NV 0x862E
#endif
#ifndef GL_MAX_TRACK_MATRICES_NV
#define GL_MAX_TRACK_MATRICES_NV 0x862F
#endif
#ifndef GL_MATRIX0_NV
#define GL_MATRIX0_NV 0x8630
#endif
#ifndef GL_MATRIX1_NV
#define GL_MATRIX1_NV 0x8631
#endif
#ifndef GL_MATRIX2_NV
#define GL_MATRIX2_NV 0x8632
#endif
#ifndef GL_MATRIX3_NV
#define GL_MATRIX3_NV 0x8633
#endif
#ifndef GL_MATRIX4_NV
#define GL_MATRIX4_NV 0x8634
#endif
#ifndef GL_MATRIX5_NV
#define GL_MATRIX5_NV 0x8635
#endif
#ifndef GL_MATRIX6_NV
#define GL_MATRIX6_NV 0x8636
#endif
#ifndef GL_MATRIX7_NV
#define GL_MATRIX7_NV 0x8637
#endif
#ifndef GL_CURRENT_MATRIX_STACK_DEPTH_NV
#define GL_CURRENT_MATRIX_STACK_DEPTH_NV 0x8640
#endif
#ifndef GL_CURRENT_MATRIX_NV
#define GL_CURRENT_MATRIX_NV 0x8641
#endif
#ifndef GL_VERTEX_PROGRAM_POINT_SIZE_NV
#define GL_VERTEX_PROGRAM_POINT_SIZE_NV 0x8642
#endif
#ifndef GL_VERTEX_PROGRAM_TWO_SIDE_NV
#define GL_VERTEX_PROGRAM_TWO_SIDE_NV 0x8643
#endif
#ifndef GL_PROGRAM_PARAMETER_NV
#define GL_PROGRAM_PARAMETER_NV 0x8644
#endif
#ifndef GL_ATTRIB_ARRAY_POINTER_NV
#define GL_ATTRIB_ARRAY_POINTER_NV 0x8645
#endif
#ifndef GL_PROGRAM_TARGET_NV
#define GL_PROGRAM_TARGET_NV 0x8646
#endif
#ifndef GL_PROGRAM_RESIDENT_NV
#define GL_PROGRAM_RESIDENT_NV 0x8647
#endif
#ifndef GL_TRACK_MATRIX_NV
#define GL_TRACK_MATRIX_NV 0x8648
#endif
#ifndef GL_TRACK_MATRIX_TRANSFORM_NV
#define GL_TRACK_MATRIX_TRANSFORM_NV 0x8649
#endif
#ifndef GL_VERTEX_PROGRAM_BINDING_NV
#define GL_VERTEX_PROGRAM_BINDING_NV 0x864A
#endif
#ifndef GL_PROGRAM_ERROR_POSITION_NV
#define GL_PROGRAM_ERROR_POSITION_NV 0x864B
#endif
#ifndef GL_VERTEX_ATTRIB_ARRAY0_NV
#define GL_VERTEX_ATTRIB_ARRAY0_NV 0x8650
#endif
#ifndef GL_VERTEX_ATTRIB_ARRAY1_NV
#define GL_VERTEX_ATTRIB_ARRAY1_NV 0x8651
#endif
#ifndef GL_VERTEX_ATTRIB_ARRAY2_NV
#define GL_VERTEX_ATTRIB_ARRAY2_NV 0x8652
#endif
#ifndef GL_VERTEX_ATTRIB_ARRAY3_NV
#define GL_VERTEX_ATTRIB_ARRAY3_NV 0x8653
#endif
#ifndef GL_VERTEX_ATTRIB_ARRAY4_NV
#define GL_VERTEX_ATTRIB_ARRAY4_NV 0x8654
#endif
#ifndef GL_VERTEX_ATTRIB_ARRAY5_NV
#define GL_VERTEX_ATTRIB_ARRAY5_NV 0x8655
#endif
#ifndef GL_VERTEX_ATTRIB_ARRAY6_NV
#define GL_VERTEX_ATTRIB_ARRAY6_NV 0x8656
#endif
#ifndef GL_VERTEX_ATTRIB_ARRAY7_NV
#define GL_VERTEX_ATTRIB_ARRAY7_NV 0x8657
#endif
#ifndef GL_VERTEX_ATTRIB_ARRAY8_NV
#define GL_VERTEX_ATTRIB_ARRAY8_NV 0x8658
#endif
#ifndef GL_VERTEX_ATTRIB_ARRAY9_NV
#define GL_VERTEX_ATTRIB_ARRAY9_NV 0x8659
#endif
#ifndef GL_VERTEX_ATTRIB_ARRAY10_NV
#define GL_VERTEX_ATTRIB_ARRAY10_NV 0x865A
#endif
#ifndef GL_VERTEX_ATTRIB_ARRAY11_NV
#define GL_VERTEX_ATTRIB_ARRAY11_NV 0x865B
#endif
#ifndef GL_VERTEX_ATTRIB_ARRAY12_NV
#define GL_VERTEX_ATTRIB_ARRAY12_NV 0x865C
#endif
#ifndef GL_VERTEX_ATTRIB_ARRAY13_NV
#define GL_VERTEX_ATTRIB_ARRAY13_NV 0x865D
#endif
#ifndef GL_VERTEX_ATTRIB_ARRAY14_NV
#define GL_VERTEX_ATTRIB_ARRAY14_NV 0x865E
#endif
#ifndef GL_VERTEX_ATTRIB_ARRAY15_NV
#define GL_VERTEX_ATTRIB_ARRAY15_NV 0x865F
#endif
#ifndef GL_MAP1_VERTEX_ATTRIB0_4_NV
#define GL_MAP1_VERTEX_ATTRIB0_4_NV 0x8660
#endif
#ifndef GL_MAP1_VERTEX_ATTRIB1_4_NV
#define GL_MAP1_VERTEX_ATTRIB1_4_NV 0x8661
#endif
#ifndef GL_MAP1_VERTEX_ATTRIB2_4_NV
#define GL_MAP1_VERTEX_ATTRIB2_4_NV 0x8662
#endif
#ifndef GL_MAP1_VERTEX_ATTRIB3_4_NV
#define GL_MAP1_VERTEX_ATTRIB3_4_NV 0x8663
#endif
#ifndef GL_MAP1_VERTEX_ATTRIB4_4_NV
#define GL_MAP1_VERTEX_ATTRIB4_4_NV 0x8664
#endif
#ifndef GL_MAP1_VERTEX_ATTRIB5_4_NV
#define GL_MAP1_VERTEX_ATTRIB5_4_NV 0x8665
#endif
#ifndef GL_MAP1_VERTEX_ATTRIB6_4_NV
#define GL_MAP1_VERTEX_ATTRIB6_4_NV 0x8666
#endif
#ifndef GL_MAP1_VERTEX_ATTRIB7_4_NV
#define GL_MAP1_VERTEX_ATTRIB7_4_NV 0x8667
#endif
#ifndef GL_MAP1_VERTEX_ATTRIB8_4_NV
#define GL_MAP1_VERTEX_ATTRIB8_4_NV 0x8668
#endif
#ifndef GL_MAP1_VERTEX_ATTRIB9_4_NV
#define GL_MAP1_VERTEX_ATTRIB9_4_NV 0x8669
#endif
#ifndef GL_MAP1_VERTEX_ATTRIB10_4_NV
#define GL_MAP1_VERTEX_ATTRIB10_4_NV 0x866A
#endif
#ifndef GL_MAP1_VERTEX_ATTRIB11_4_NV
#define GL_MAP1_VERTEX_ATTRIB11_4_NV 0x866B
#endif
#ifndef GL_MAP1_VERTEX_ATTRIB12_4_NV
#define GL_MAP1_VERTEX_ATTRIB12_4_NV 0x866C
#endif
#ifndef GL_MAP1_VERTEX_ATTRIB13_4_NV
#define GL_MAP1_VERTEX_ATTRIB13_4_NV 0x866D
#endif
#ifndef GL_MAP1_VERTEX_ATTRIB14_4_NV
#define GL_MAP1_VERTEX_ATTRIB14_4_NV 0x866E
#endif
#ifndef GL_MAP1_VERTEX_ATTRIB15_4_NV
#define GL_MAP1_VERTEX_ATTRIB15_4_NV 0x866F
#endif
#ifndef GL_MAP2_VERTEX_ATTRIB0_4_NV
#define GL_MAP2_VERTEX_ATTRIB0_4_NV 0x8670
#endif
#ifndef GL_MAP2_VERTEX_ATTRIB1_4_NV
#define GL_MAP2_VERTEX_ATTRIB1_4_NV 0x8671
#endif
#ifndef GL_MAP2_VERTEX_ATTRIB2_4_NV
#define GL_MAP2_VERTEX_ATTRIB2_4_NV 0x8672
#endif
#ifndef GL_MAP2_VERTEX_ATTRIB3_4_NV
#define GL_MAP2_VERTEX_ATTRIB3_4_NV 0x8673
#endif
#ifndef GL_MAP2_VERTEX_ATTRIB4_4_NV
#define GL_MAP2_VERTEX_ATTRIB4_4_NV 0x8674
#endif
#ifndef GL_MAP2_VERTEX_ATTRIB5_4_NV
#define GL_MAP2_VERTEX_ATTRIB5_4_NV 0x8675
#endif
#ifndef GL_MAP2_VERTEX_ATTRIB6_4_NV
#define GL_MAP2_VERTEX_ATTRIB6_4_NV 0x8676
#endif
#ifndef GL_MAP2_VERTEX_ATTRIB7_4_NV
#define GL_MAP2_VERTEX_ATTRIB7_4_NV 0x8677
#endif
#ifndef GL_MAP2_VERTEX_ATTRIB8_4_NV
#define GL_MAP2_VERTEX_ATTRIB8_4_NV 0x8678
#endif
#ifndef GL_MAP2_VERTEX_ATTRIB9_4_NV
#define GL_MAP2_VERTEX_ATTRIB9_4_NV 0x8679
#endif
#ifndef GL_MAP2_VERTEX_ATTRIB10_4_NV
#define GL_MAP2_VERTEX_ATTRIB10_4_NV 0x867A
#endif
#ifndef GL_MAP2_VERTEX_ATTRIB11_4_NV
#define GL_MAP2_VERTEX_ATTRIB11_4_NV 0x867B
#endif
#ifndef GL_MAP2_VERTEX_ATTRIB12_4_NV
#define GL_MAP2_VERTEX_ATTRIB12_4_NV 0x867C
#endif
#ifndef GL_MAP2_VERTEX_ATTRIB13_4_NV
#define GL_MAP2_VERTEX_ATTRIB13_4_NV 0x867D
#endif
#ifndef GL_MAP2_VERTEX_ATTRIB14_4_NV
#define GL_MAP2_VERTEX_ATTRIB14_4_NV 0x867E
#endif
#ifndef GL_MAP2_VERTEX_ATTRIB15_4_NV
#define GL_MAP2_VERTEX_ATTRIB15_4_NV 0x867F
#endif
#ifndef GL_TEXTURE_MAX_CLAMP_S_SGIX
#define GL_TEXTURE_MAX_CLAMP_S_SGIX 0x8369
#endif
#ifndef GL_TEXTURE_MAX_CLAMP_T_SGIX
#define GL_TEXTURE_MAX_CLAMP_T_SGIX 0x836A
#endif
#ifndef GL_TEXTURE_MAX_CLAMP_R_SGIX
#define GL_TEXTURE_MAX_CLAMP_R_SGIX 0x836B
#endif
#ifndef GL_SCALEBIAS_HINT_SGIX
#define GL_SCALEBIAS_HINT_SGIX 0x8322
#endif
#ifndef GL_INTERLACE_OML
#define GL_INTERLACE_OML 0x8980
#endif
#ifndef GL_INTERLACE_READ_OML
#define GL_INTERLACE_READ_OML 0x8981
#endif
#ifndef GL_FORMAT_SUBSAMPLE_24_24_OML
#define GL_FORMAT_SUBSAMPLE_24_24_OML 0x8982
#endif
#ifndef GL_FORMAT_SUBSAMPLE_244_244_OML
#define GL_FORMAT_SUBSAMPLE_244_244_OML 0x8983
#endif
#ifndef GL_PACK_RESAMPLE_OML
#define GL_PACK_RESAMPLE_OML 0x8984
#endif
#ifndef GL_UNPACK_RESAMPLE_OML
#define GL_UNPACK_RESAMPLE_OML 0x8985
#endif
#ifndef GL_RESAMPLE_REPLICATE_OML
#define GL_RESAMPLE_REPLICATE_OML 0x8986
#endif
#ifndef GL_RESAMPLE_ZERO_FILL_OML
#define GL_RESAMPLE_ZERO_FILL_OML 0x8987
#endif
#ifndef GL_RESAMPLE_AVERAGE_OML
#define GL_RESAMPLE_AVERAGE_OML 0x8988
#endif
#ifndef GL_RESAMPLE_DECIMATE_OML
#define GL_RESAMPLE_DECIMATE_OML 0x8989
#endif
#ifndef GL_DEPTH_STENCIL_TO_RGBA_NV
#define GL_DEPTH_STENCIL_TO_RGBA_NV 0x886E
#endif
#ifndef GL_DEPTH_STENCIL_TO_BGRA_NV
#define GL_DEPTH_STENCIL_TO_BGRA_NV 0x886F
#endif
#ifndef GL_BUMP_ROT_MATRIX_ATI
#define GL_BUMP_ROT_MATRIX_ATI 0x8775
#endif
#ifndef GL_BUMP_ROT_MATRIX_SIZE_ATI
#define GL_BUMP_ROT_MATRIX_SIZE_ATI 0x8776
#endif
#ifndef GL_BUMP_NUM_TEX_UNITS_ATI
#define GL_BUMP_NUM_TEX_UNITS_ATI 0x8777
#endif
#ifndef GL_BUMP_TEX_UNITS_ATI
#define GL_BUMP_TEX_UNITS_ATI 0x8778
#endif
#ifndef GL_DUDV_ATI
#define GL_DUDV_ATI 0x8779
#endif
#ifndef GL_DU8DV8_ATI
#define GL_DU8DV8_ATI 0x877A
#endif
#ifndef GL_BUMP_ENVMAP_ATI
#define GL_BUMP_ENVMAP_ATI 0x877B
#endif
#ifndef GL_BUMP_TARGET_ATI
#define GL_BUMP_TARGET_ATI 0x877C
#endif
#ifndef GL_FRAGMENT_SHADER_ATI
#define GL_FRAGMENT_SHADER_ATI 0x8920
#endif
#ifndef GL_REG_0_ATI
#define GL_REG_0_ATI 0x8921
#endif
#ifndef GL_REG_1_ATI
#define GL_REG_1_ATI 0x8922
#endif
#ifndef GL_REG_2_ATI
#define GL_REG_2_ATI 0x8923
#endif
#ifndef GL_REG_3_ATI
#define GL_REG_3_ATI 0x8924
#endif
#ifndef GL_REG_4_ATI
#define GL_REG_4_ATI 0x8925
#endif
#ifndef GL_REG_5_ATI
#define GL_REG_5_ATI 0x8926
#endif
#ifndef GL_REG_6_ATI
#define GL_REG_6_ATI 0x8927
#endif
#ifndef GL_REG_7_ATI
#define GL_REG_7_ATI 0x8928
#endif
#ifndef GL_REG_8_ATI
#define GL_REG_8_ATI 0x8929
#endif
#ifndef GL_REG_9_ATI
#define GL_REG_9_ATI 0x892A
#endif
#ifndef GL_REG_10_ATI
#define GL_REG_10_ATI 0x892B
#endif
#ifndef GL_REG_11_ATI
#define GL_REG_11_ATI 0x892C
#endif
#ifndef GL_REG_12_ATI
#define GL_REG_12_ATI 0x892D
#endif
#ifndef GL_REG_13_ATI
#define GL_REG_13_ATI 0x892E
#endif
#ifndef GL_REG_14_ATI
#define GL_REG_14_ATI 0x892F
#endif
#ifndef GL_REG_15_ATI
#define GL_REG_15_ATI 0x8930
#endif
#ifndef GL_REG_16_ATI
#define GL_REG_16_ATI 0x8931
#endif
#ifndef GL_REG_17_ATI
#define GL_REG_17_ATI 0x8932
#endif
#ifndef GL_REG_18_ATI
#define GL_REG_18_ATI 0x8933
#endif
#ifndef GL_REG_19_ATI
#define GL_REG_19_ATI 0x8934
#endif
#ifndef GL_REG_20_ATI
#define GL_REG_20_ATI 0x8935
#endif
#ifndef GL_REG_21_ATI
#define GL_REG_21_ATI 0x8936
#endif
#ifndef GL_REG_22_ATI
#define GL_REG_22_ATI 0x8937
#endif
#ifndef GL_REG_23_ATI
#define GL_REG_23_ATI 0x8938
#endif
#ifndef GL_REG_24_ATI
#define GL_REG_24_ATI 0x8939
#endif
#ifndef GL_REG_25_ATI
#define GL_REG_25_ATI 0x893A
#endif
#ifndef GL_REG_26_ATI
#define GL_REG_26_ATI 0x893B
#endif
#ifndef GL_REG_27_ATI
#define GL_REG_27_ATI 0x893C
#endif
#ifndef GL_REG_28_ATI
#define GL_REG_28_ATI 0x893D
#endif
#ifndef GL_REG_29_ATI
#define GL_REG_29_ATI 0x893E
#endif
#ifndef GL_REG_30_ATI
#define GL_REG_30_ATI 0x893F
#endif
#ifndef GL_REG_31_ATI
#define GL_REG_31_ATI 0x8940
#endif
#ifndef GL_CON_0_ATI
#define GL_CON_0_ATI 0x8941
#endif
#ifndef GL_CON_1_ATI
#define GL_CON_1_ATI 0x8942
#endif
#ifndef GL_CON_2_ATI
#define GL_CON_2_ATI 0x8943
#endif
#ifndef GL_CON_3_ATI
#define GL_CON_3_ATI 0x8944
#endif
#ifndef GL_CON_4_ATI
#define GL_CON_4_ATI 0x8945
#endif
#ifndef GL_CON_5_ATI
#define GL_CON_5_ATI 0x8946
#endif
#ifndef GL_CON_6_ATI
#define GL_CON_6_ATI 0x8947
#endif
#ifndef GL_CON_7_ATI
#define GL_CON_7_ATI 0x8948
#endif
#ifndef GL_CON_8_ATI
#define GL_CON_8_ATI 0x8949
#endif
#ifndef GL_CON_9_ATI
#define GL_CON_9_ATI 0x894A
#endif
#ifndef GL_CON_10_ATI
#define GL_CON_10_ATI 0x894B
#endif
#ifndef GL_CON_11_ATI
#define GL_CON_11_ATI 0x894C
#endif
#ifndef GL_CON_12_ATI
#define GL_CON_12_ATI 0x894D
#endif
#ifndef GL_CON_13_ATI
#define GL_CON_13_ATI 0x894E
#endif
#ifndef GL_CON_14_ATI
#define GL_CON_14_ATI 0x894F
#endif
#ifndef GL_CON_15_ATI
#define GL_CON_15_ATI 0x8950
#endif
#ifndef GL_CON_16_ATI
#define GL_CON_16_ATI 0x8951
#endif
#ifndef GL_CON_17_ATI
#define GL_CON_17_ATI 0x8952
#endif
#ifndef GL_CON_18_ATI
#define GL_CON_18_ATI 0x8953
#endif
#ifndef GL_CON_19_ATI
#define GL_CON_19_ATI 0x8954
#endif
#ifndef GL_CON_20_ATI
#define GL_CON_20_ATI 0x8955
#endif
#ifndef GL_CON_21_ATI
#define GL_CON_21_ATI 0x8956
#endif
#ifndef GL_CON_22_ATI
#define GL_CON_22_ATI 0x8957
#endif
#ifndef GL_CON_23_ATI
#define GL_CON_23_ATI 0x8958
#endif
#ifndef GL_CON_24_ATI
#define GL_CON_24_ATI 0x8959
#endif
#ifndef GL_CON_25_ATI
#define GL_CON_25_ATI 0x895A
#endif
#ifndef GL_CON_26_ATI
#define GL_CON_26_ATI 0x895B
#endif
#ifndef GL_CON_27_ATI
#define GL_CON_27_ATI 0x895C
#endif
#ifndef GL_CON_28_ATI
#define GL_CON_28_ATI 0x895D
#endif
#ifndef GL_CON_29_ATI
#define GL_CON_29_ATI 0x895E
#endif
#ifndef GL_CON_30_ATI
#define GL_CON_30_ATI 0x895F
#endif
#ifndef GL_CON_31_ATI
#define GL_CON_31_ATI 0x8960
#endif
#ifndef GL_MOV_ATI
#define GL_MOV_ATI 0x8961
#endif
#ifndef GL_ADD_ATI
#define GL_ADD_ATI 0x8963
#endif
#ifndef GL_MUL_ATI
#define GL_MUL_ATI 0x8964
#endif
#ifndef GL_SUB_ATI
#define GL_SUB_ATI 0x8965
#endif
#ifndef GL_DOT3_ATI
#define GL_DOT3_ATI 0x8966
#endif
#ifndef GL_DOT4_ATI
#define GL_DOT4_ATI 0x8967
#endif
#ifndef GL_MAD_ATI
#define GL_MAD_ATI 0x8968
#endif
#ifndef GL_LERP_ATI
#define GL_LERP_ATI 0x8969
#endif
#ifndef GL_CND_ATI
#define GL_CND_ATI 0x896A
#endif
#ifndef GL_CND0_ATI
#define GL_CND0_ATI 0x896B
#endif
#ifndef GL_DOT2_ADD_ATI
#define GL_DOT2_ADD_ATI 0x896C
#endif
#ifndef GL_SECONDARY_INTERPOLATOR_ATI
#define GL_SECONDARY_INTERPOLATOR_ATI 0x896D
#endif
#ifndef GL_NUM_FRAGMENT_REGISTERS_ATI
#define GL_NUM_FRAGMENT_REGISTERS_ATI 0x896E
#endif
#ifndef GL_NUM_FRAGMENT_CONSTANTS_ATI
#define GL_NUM_FRAGMENT_CONSTANTS_ATI 0x896F
#endif
#ifndef GL_NUM_PASSES_ATI
#define GL_NUM_PASSES_ATI 0x8970
#endif
#ifndef GL_NUM_INSTRUCTIONS_PER_PASS_ATI
#define GL_NUM_INSTRUCTIONS_PER_PASS_ATI 0x8971
#endif
#ifndef GL_NUM_INSTRUCTIONS_TOTAL_ATI
#define GL_NUM_INSTRUCTIONS_TOTAL_ATI 0x8972
#endif
#ifndef GL_NUM_INPUT_INTERPOLATOR_COMPONENTS_ATI
#define GL_NUM_INPUT_INTERPOLATOR_COMPONENTS_ATI 0x8973
#endif
#ifndef GL_NUM_LOOPBACK_COMPONENTS_ATI
#define GL_NUM_LOOPBACK_COMPONENTS_ATI 0x8974
#endif
#ifndef GL_COLOR_ALPHA_PAIRING_ATI
#define GL_COLOR_ALPHA_PAIRING_ATI 0x8975
#endif
#ifndef GL_SWIZZLE_STR_ATI
#define GL_SWIZZLE_STR_ATI 0x8976
#endif
#ifndef GL_SWIZZLE_STQ_ATI
#define GL_SWIZZLE_STQ_ATI 0x8977
#endif
#ifndef GL_SWIZZLE_STR_DR_ATI
#define GL_SWIZZLE_STR_DR_ATI 0x8978
#endif
#ifndef GL_SWIZZLE_STQ_DQ_ATI
#define GL_SWIZZLE_STQ_DQ_ATI 0x8979
#endif
#ifndef GL_SWIZZLE_STRQ_ATI
#define GL_SWIZZLE_STRQ_ATI 0x897A
#endif
#ifndef GL_SWIZZLE_STRQ_DQ_ATI
#define GL_SWIZZLE_STRQ_DQ_ATI 0x897B
#endif
#ifndef GL_RED_BIT_ATI
#define GL_RED_BIT_ATI 0x00000001
#endif
#ifndef GL_GREEN_BIT_ATI
#define GL_GREEN_BIT_ATI 0x00000002
#endif
#ifndef GL_BLUE_BIT_ATI
#define GL_BLUE_BIT_ATI 0x00000004
#endif
#ifndef GL_2X_BIT_ATI
#define GL_2X_BIT_ATI 0x00000001
#endif
#ifndef GL_4X_BIT_ATI
#define GL_4X_BIT_ATI 0x00000002
#endif
#ifndef GL_8X_BIT_ATI
#define GL_8X_BIT_ATI 0x00000004
#endif
#ifndef GL_HALF_BIT_ATI
#define GL_HALF_BIT_ATI 0x00000008
#endif
#ifndef GL_QUARTER_BIT_ATI
#define GL_QUARTER_BIT_ATI 0x00000010
#endif
#ifndef GL_EIGHTH_BIT_ATI
#define GL_EIGHTH_BIT_ATI 0x00000020
#endif
#ifndef GL_SATURATE_BIT_ATI
#define GL_SATURATE_BIT_ATI 0x00000040
#endif
#ifndef GL_2X_BIT_ATI
#define GL_2X_BIT_ATI 0x00000001
#endif
#ifndef GL_COMP_BIT_ATI
#define GL_COMP_BIT_ATI 0x00000002
#endif
#ifndef GL_NEGATE_BIT_ATI
#define GL_NEGATE_BIT_ATI 0x00000004
#endif
#ifndef GL_BIAS_BIT_ATI
#define GL_BIAS_BIT_ATI 0x00000008
#endif
#ifndef GL_PN_TRIANGLES_ATI
#define GL_PN_TRIANGLES_ATI 0x87F0
#endif
#ifndef GL_MAX_PN_TRIANGLES_TESSELATION_LEVEL_ATI
#define GL_MAX_PN_TRIANGLES_TESSELATION_LEVEL_ATI 0x87F1
#endif
#ifndef GL_PN_TRIANGLES_POINT_MODE_ATI
#define GL_PN_TRIANGLES_POINT_MODE_ATI 0x87F2
#endif
#ifndef GL_PN_TRIANGLES_NORMAL_MODE_ATI
#define GL_PN_TRIANGLES_NORMAL_MODE_ATI 0x87F3
#endif
#ifndef GL_PN_TRIANGLES_TESSELATION_LEVEL_ATI
#define GL_PN_TRIANGLES_TESSELATION_LEVEL_ATI 0x87F4
#endif
#ifndef GL_PN_TRIANGLES_POINT_MODE_LINEAR_ATI
#define GL_PN_TRIANGLES_POINT_MODE_LINEAR_ATI 0x87F5
#endif
#ifndef GL_PN_TRIANGLES_POINT_MODE_CUBIC_ATI
#define GL_PN_TRIANGLES_POINT_MODE_CUBIC_ATI 0x87F6
#endif
#ifndef GL_PN_TRIANGLES_NORMAL_MODE_LINEAR_ATI
#define GL_PN_TRIANGLES_NORMAL_MODE_LINEAR_ATI 0x87F7
#endif
#ifndef GL_PN_TRIANGLES_NORMAL_MODE_QUADRATIC_ATI
#define GL_PN_TRIANGLES_NORMAL_MODE_QUADRATIC_ATI 0x87F8
#endif
#ifndef GL_STATIC_ATI
#define GL_STATIC_ATI 0x8760
#endif
#ifndef GL_DYNAMIC_ATI
#define GL_DYNAMIC_ATI 0x8761
#endif
#ifndef GL_PRESERVE_ATI
#define GL_PRESERVE_ATI 0x8762
#endif
#ifndef GL_DISCARD_ATI
#define GL_DISCARD_ATI 0x8763
#endif
#ifndef GL_OBJECT_BUFFER_SIZE_ATI
#define GL_OBJECT_BUFFER_SIZE_ATI 0x8764
#endif
#ifndef GL_OBJECT_BUFFER_USAGE_ATI
#define GL_OBJECT_BUFFER_USAGE_ATI 0x8765
#endif
#ifndef GL_ARRAY_OBJECT_BUFFER_ATI
#define GL_ARRAY_OBJECT_BUFFER_ATI 0x8766
#endif
#ifndef GL_ARRAY_OBJECT_OFFSET_ATI
#define GL_ARRAY_OBJECT_OFFSET_ATI 0x8767
#endif
#ifndef GL_VERTEX_SHADER_EXT
#define GL_VERTEX_SHADER_EXT 0x8780
#endif
#ifndef GL_VERTEX_SHADER_BINDING_EXT
#define GL_VERTEX_SHADER_BINDING_EXT 0x8781
#endif
#ifndef GL_OP_INDEX_EXT
#define GL_OP_INDEX_EXT 0x8782
#endif
#ifndef GL_OP_NEGATE_EXT
#define GL_OP_NEGATE_EXT 0x8783
#endif
#ifndef GL_OP_DOT3_EXT
#define GL_OP_DOT3_EXT 0x8784
#endif
#ifndef GL_OP_DOT4_EXT
#define GL_OP_DOT4_EXT 0x8785
#endif
#ifndef GL_OP_MUL_EXT
#define GL_OP_MUL_EXT 0x8786
#endif
#ifndef GL_OP_ADD_EXT
#define GL_OP_ADD_EXT 0x8787
#endif
#ifndef GL_OP_MADD_EXT
#define GL_OP_MADD_EXT 0x8788
#endif
#ifndef GL_OP_FRAC_EXT
#define GL_OP_FRAC_EXT 0x8789
#endif
#ifndef GL_OP_MAX_EXT
#define GL_OP_MAX_EXT 0x878A
#endif
#ifndef GL_OP_MIN_EXT
#define GL_OP_MIN_EXT 0x878B
#endif
#ifndef GL_OP_SET_GE_EXT
#define GL_OP_SET_GE_EXT 0x878C
#endif
#ifndef GL_OP_SET_LT_EXT
#define GL_OP_SET_LT_EXT 0x878D
#endif
#ifndef GL_OP_CLAMP_EXT
#define GL_OP_CLAMP_EXT 0x878E
#endif
#ifndef GL_OP_FLOOR_EXT
#define GL_OP_FLOOR_EXT 0x878F
#endif
#ifndef GL_OP_ROUND_EXT
#define GL_OP_ROUND_EXT 0x8790
#endif
#ifndef GL_OP_EXP_BASE_2_EXT
#define GL_OP_EXP_BASE_2_EXT 0x8791
#endif
#ifndef GL_OP_LOG_BASE_2_EXT
#define GL_OP_LOG_BASE_2_EXT 0x8792
#endif
#ifndef GL_OP_POWER_EXT
#define GL_OP_POWER_EXT 0x8793
#endif
#ifndef GL_OP_RECIP_EXT
#define GL_OP_RECIP_EXT 0x8794
#endif
#ifndef GL_OP_RECIP_SQRT_EXT
#define GL_OP_RECIP_SQRT_EXT 0x8795
#endif
#ifndef GL_OP_SUB_EXT
#define GL_OP_SUB_EXT 0x8796
#endif
#ifndef GL_OP_CROSS_PRODUCT_EXT
#define GL_OP_CROSS_PRODUCT_EXT 0x8797
#endif
#ifndef GL_OP_MULTIPLY_MATRIX_EXT
#define GL_OP_MULTIPLY_MATRIX_EXT 0x8798
#endif
#ifndef GL_OP_MOV_EXT
#define GL_OP_MOV_EXT 0x8799
#endif
#ifndef GL_OUTPUT_VERTEX_EXT
#define GL_OUTPUT_VERTEX_EXT 0x879A
#endif
#ifndef GL_OUTPUT_COLOR0_EXT
#define GL_OUTPUT_COLOR0_EXT 0x879B
#endif
#ifndef GL_OUTPUT_COLOR1_EXT
#define GL_OUTPUT_COLOR1_EXT 0x879C
#endif
#ifndef GL_OUTPUT_TEXTURE_COORD0_EXT
#define GL_OUTPUT_TEXTURE_COORD0_EXT 0x879D
#endif
#ifndef GL_OUTPUT_TEXTURE_COORD1_EXT
#define GL_OUTPUT_TEXTURE_COORD1_EXT 0x879E
#endif
#ifndef GL_OUTPUT_TEXTURE_COORD2_EXT
#define GL_OUTPUT_TEXTURE_COORD2_EXT 0x879F
#endif
#ifndef GL_OUTPUT_TEXTURE_COORD3_EXT
#define GL_OUTPUT_TEXTURE_COORD3_EXT 0x87A0
#endif
#ifndef GL_OUTPUT_TEXTURE_COORD4_EXT
#define GL_OUTPUT_TEXTURE_COORD4_EXT 0x87A1
#endif
#ifndef GL_OUTPUT_TEXTURE_COORD5_EXT
#define GL_OUTPUT_TEXTURE_COORD5_EXT 0x87A2
#endif
#ifndef GL_OUTPUT_TEXTURE_COORD6_EXT
#define GL_OUTPUT_TEXTURE_COORD6_EXT 0x87A3
#endif
#ifndef GL_OUTPUT_TEXTURE_COORD7_EXT
#define GL_OUTPUT_TEXTURE_COORD7_EXT 0x87A4
#endif
#ifndef GL_OUTPUT_TEXTURE_COORD8_EXT
#define GL_OUTPUT_TEXTURE_COORD8_EXT 0x87A5
#endif
#ifndef GL_OUTPUT_TEXTURE_COORD9_EXT
#define GL_OUTPUT_TEXTURE_COORD9_EXT 0x87A6
#endif
#ifndef GL_OUTPUT_TEXTURE_COORD10_EXT
#define GL_OUTPUT_TEXTURE_COORD10_EXT 0x87A7
#endif
#ifndef GL_OUTPUT_TEXTURE_COORD11_EXT
#define GL_OUTPUT_TEXTURE_COORD11_EXT 0x87A8
#endif
#ifndef GL_OUTPUT_TEXTURE_COORD12_EXT
#define GL_OUTPUT_TEXTURE_COORD12_EXT 0x87A9
#endif
#ifndef GL_OUTPUT_TEXTURE_COORD13_EXT
#define GL_OUTPUT_TEXTURE_COORD13_EXT 0x87AA
#endif
#ifndef GL_OUTPUT_TEXTURE_COORD14_EXT
#define GL_OUTPUT_TEXTURE_COORD14_EXT 0x87AB
#endif
#ifndef GL_OUTPUT_TEXTURE_COORD15_EXT
#define GL_OUTPUT_TEXTURE_COORD15_EXT 0x87AC
#endif
#ifndef GL_OUTPUT_TEXTURE_COORD16_EXT
#define GL_OUTPUT_TEXTURE_COORD16_EXT 0x87AD
#endif
#ifndef GL_OUTPUT_TEXTURE_COORD17_EXT
#define GL_OUTPUT_TEXTURE_COORD17_EXT 0x87AE
#endif
#ifndef GL_OUTPUT_TEXTURE_COORD18_EXT
#define GL_OUTPUT_TEXTURE_COORD18_EXT 0x87AF
#endif
#ifndef GL_OUTPUT_TEXTURE_COORD19_EXT
#define GL_OUTPUT_TEXTURE_COORD19_EXT 0x87B0
#endif
#ifndef GL_OUTPUT_TEXTURE_COORD20_EXT
#define GL_OUTPUT_TEXTURE_COORD20_EXT 0x87B1
#endif
#ifndef GL_OUTPUT_TEXTURE_COORD21_EXT
#define GL_OUTPUT_TEXTURE_COORD21_EXT 0x87B2
#endif
#ifndef GL_OUTPUT_TEXTURE_COORD22_EXT
#define GL_OUTPUT_TEXTURE_COORD22_EXT 0x87B3
#endif
#ifndef GL_OUTPUT_TEXTURE_COORD23_EXT
#define GL_OUTPUT_TEXTURE_COORD23_EXT 0x87B4
#endif
#ifndef GL_OUTPUT_TEXTURE_COORD24_EXT
#define GL_OUTPUT_TEXTURE_COORD24_EXT 0x87B5
#endif
#ifndef GL_OUTPUT_TEXTURE_COORD25_EXT
#define GL_OUTPUT_TEXTURE_COORD25_EXT 0x87B6
#endif
#ifndef GL_OUTPUT_TEXTURE_COORD26_EXT
#define GL_OUTPUT_TEXTURE_COORD26_EXT 0x87B7
#endif
#ifndef GL_OUTPUT_TEXTURE_COORD27_EXT
#define GL_OUTPUT_TEXTURE_COORD27_EXT 0x87B8
#endif
#ifndef GL_OUTPUT_TEXTURE_COORD28_EXT
#define GL_OUTPUT_TEXTURE_COORD28_EXT 0x87B9
#endif
#ifndef GL_OUTPUT_TEXTURE_COORD29_EXT
#define GL_OUTPUT_TEXTURE_COORD29_EXT 0x87BA
#endif
#ifndef GL_OUTPUT_TEXTURE_COORD30_EXT
#define GL_OUTPUT_TEXTURE_COORD30_EXT 0x87BB
#endif
#ifndef GL_OUTPUT_TEXTURE_COORD31_EXT
#define GL_OUTPUT_TEXTURE_COORD31_EXT 0x87BC
#endif
#ifndef GL_OUTPUT_FOG_EXT
#define GL_OUTPUT_FOG_EXT 0x87BD
#endif
#ifndef GL_SCALAR_EXT
#define GL_SCALAR_EXT 0x87BE
#endif
#ifndef GL_VECTOR_EXT
#define GL_VECTOR_EXT 0x87BF
#endif
#ifndef GL_MATRIX_EXT
#define GL_MATRIX_EXT 0x87C0
#endif
#ifndef GL_VARIANT_EXT
#define GL_VARIANT_EXT 0x87C1
#endif
#ifndef GL_INVARIANT_EXT
#define GL_INVARIANT_EXT 0x87C2
#endif
#ifndef GL_LOCAL_CONSTANT_EXT
#define GL_LOCAL_CONSTANT_EXT 0x87C3
#endif
#ifndef GL_LOCAL_EXT
#define GL_LOCAL_EXT 0x87C4
#endif
#ifndef GL_MAX_VERTEX_SHADER_INSTRUCTIONS_EXT
#define GL_MAX_VERTEX_SHADER_INSTRUCTIONS_EXT 0x87C5
#endif
#ifndef GL_MAX_VERTEX_SHADER_VARIANTS_EXT
#define GL_MAX_VERTEX_SHADER_VARIANTS_EXT 0x87C6
#endif
#ifndef GL_MAX_VERTEX_SHADER_INVARIANTS_EXT
#define GL_MAX_VERTEX_SHADER_INVARIANTS_EXT 0x87C7
#endif
#ifndef GL_MAX_VERTEX_SHADER_LOCAL_CONSTANTS_EXT
#define GL_MAX_VERTEX_SHADER_LOCAL_CONSTANTS_EXT 0x87C8
#endif
#ifndef GL_MAX_VERTEX_SHADER_LOCALS_EXT
#define GL_MAX_VERTEX_SHADER_LOCALS_EXT 0x87C9
#endif
#ifndef GL_MAX_OPTIMIZED_VERTEX_SHADER_INSTRUCTIONS_EXT
#define GL_MAX_OPTIMIZED_VERTEX_SHADER_INSTRUCTIONS_EXT 0x87CA
#endif
#ifndef GL_MAX_OPTIMIZED_VERTEX_SHADER_VARIANTS_EXT
#define GL_MAX_OPTIMIZED_VERTEX_SHADER_VARIANTS_EXT 0x87CB
#endif
#ifndef GL_MAX_OPTIMIZED_VERTEX_SHADER_LOCAL_CONSTANTS_EXT
#define GL_MAX_OPTIMIZED_VERTEX_SHADER_LOCAL_CONSTANTS_EXT 0x87CC
#endif
#ifndef GL_MAX_OPTIMIZED_VERTEX_SHADER_INVARIANTS_EXT
#define GL_MAX_OPTIMIZED_VERTEX_SHADER_INVARIANTS_EXT 0x87CD
#endif
#ifndef GL_MAX_OPTIMIZED_VERTEX_SHADER_LOCALS_EXT
#define GL_MAX_OPTIMIZED_VERTEX_SHADER_LOCALS_EXT 0x87CE
#endif
#ifndef GL_VERTEX_SHADER_INSTRUCTIONS_EXT
#define GL_VERTEX_SHADER_INSTRUCTIONS_EXT 0x87CF
#endif
#ifndef GL_VERTEX_SHADER_VARIANTS_EXT
#define GL_VERTEX_SHADER_VARIANTS_EXT 0x87D0
#endif
#ifndef GL_VERTEX_SHADER_INVARIANTS_EXT
#define GL_VERTEX_SHADER_INVARIANTS_EXT 0x87D1
#endif
#ifndef GL_VERTEX_SHADER_LOCAL_CONSTANTS_EXT
#define GL_VERTEX_SHADER_LOCAL_CONSTANTS_EXT 0x87D2
#endif
#ifndef GL_VERTEX_SHADER_LOCALS_EXT
#define GL_VERTEX_SHADER_LOCALS_EXT 0x87D3
#endif
#ifndef GL_VERTEX_SHADER_OPTIMIZED_EXT
#define GL_VERTEX_SHADER_OPTIMIZED_EXT 0x87D4
#endif
#ifndef GL_X_EXT
#define GL_X_EXT 0x87D5
#endif
#ifndef GL_Y_EXT
#define GL_Y_EXT 0x87D6
#endif
#ifndef GL_Z_EXT
#define GL_Z_EXT 0x87D7
#endif
#ifndef GL_W_EXT
#define GL_W_EXT 0x87D8
#endif
#ifndef GL_NEGATIVE_X_EXT
#define GL_NEGATIVE_X_EXT 0x87D9
#endif
#ifndef GL_NEGATIVE_Y_EXT
#define GL_NEGATIVE_Y_EXT 0x87DA
#endif
#ifndef GL_NEGATIVE_Z_EXT
#define GL_NEGATIVE_Z_EXT 0x87DB
#endif
#ifndef GL_NEGATIVE_W_EXT
#define GL_NEGATIVE_W_EXT 0x87DC
#endif
#ifndef GL_ZERO_EXT
#define GL_ZERO_EXT 0x87DD
#endif
#ifndef GL_ONE_EXT
#define GL_ONE_EXT 0x87DE
#endif
#ifndef GL_NEGATIVE_ONE_EXT
#define GL_NEGATIVE_ONE_EXT 0x87DF
#endif
#ifndef GL_NORMALIZED_RANGE_EXT
#define GL_NORMALIZED_RANGE_EXT 0x87E0
#endif
#ifndef GL_FULL_RANGE_EXT
#define GL_FULL_RANGE_EXT 0x87E1
#endif
#ifndef GL_CURRENT_VERTEX_EXT
#define GL_CURRENT_VERTEX_EXT 0x87E2
#endif
#ifndef GL_MVP_MATRIX_EXT
#define GL_MVP_MATRIX_EXT 0x87E3
#endif
#ifndef GL_VARIANT_VALUE_EXT
#define GL_VARIANT_VALUE_EXT 0x87E4
#endif
#ifndef GL_VARIANT_DATATYPE_EXT
#define GL_VARIANT_DATATYPE_EXT 0x87E5
#endif
#ifndef GL_VARIANT_ARRAY_STRIDE_EXT
#define GL_VARIANT_ARRAY_STRIDE_EXT 0x87E6
#endif
#ifndef GL_VARIANT_ARRAY_TYPE_EXT
#define GL_VARIANT_ARRAY_TYPE_EXT 0x87E7
#endif
#ifndef GL_VARIANT_ARRAY_EXT
#define GL_VARIANT_ARRAY_EXT 0x87E8
#endif
#ifndef GL_VARIANT_ARRAY_POINTER_EXT
#define GL_VARIANT_ARRAY_POINTER_EXT 0x87E9
#endif
#ifndef GL_INVARIANT_VALUE_EXT
#define GL_INVARIANT_VALUE_EXT 0x87EA
#endif
#ifndef GL_INVARIANT_DATATYPE_EXT
#define GL_INVARIANT_DATATYPE_EXT 0x87EB
#endif
#ifndef GL_LOCAL_CONSTANT_VALUE_EXT
#define GL_LOCAL_CONSTANT_VALUE_EXT 0x87EC
#endif
#ifndef GL_LOCAL_CONSTANT_DATATYPE_EXT
#define GL_LOCAL_CONSTANT_DATATYPE_EXT 0x87ED
#endif
#ifndef GL_MAX_VERTEX_STREAMS_ATI
#define GL_MAX_VERTEX_STREAMS_ATI 0x876B
#endif
#ifndef GL_VERTEX_STREAM0_ATI
#define GL_VERTEX_STREAM0_ATI 0x876C
#endif
#ifndef GL_VERTEX_STREAM1_ATI
#define GL_VERTEX_STREAM1_ATI 0x876D
#endif
#ifndef GL_VERTEX_STREAM2_ATI
#define GL_VERTEX_STREAM2_ATI 0x876E
#endif
#ifndef GL_VERTEX_STREAM3_ATI
#define GL_VERTEX_STREAM3_ATI 0x876F
#endif
#ifndef GL_VERTEX_STREAM4_ATI
#define GL_VERTEX_STREAM4_ATI 0x8770
#endif
#ifndef GL_VERTEX_STREAM5_ATI
#define GL_VERTEX_STREAM5_ATI 0x8771
#endif
#ifndef GL_VERTEX_STREAM6_ATI
#define GL_VERTEX_STREAM6_ATI 0x8772
#endif
#ifndef GL_VERTEX_STREAM7_ATI
#define GL_VERTEX_STREAM7_ATI 0x8773
#endif
#ifndef GL_VERTEX_SOURCE_ATI
#define GL_VERTEX_SOURCE_ATI 0x8774
#endif
#ifndef GL_ELEMENT_ARRAY_ATI
#define GL_ELEMENT_ARRAY_ATI 0x8768
#endif
#ifndef GL_ELEMENT_ARRAY_TYPE_ATI
#define GL_ELEMENT_ARRAY_TYPE_ATI 0x8769
#endif
#ifndef GL_ELEMENT_ARRAY_POINTER_ATI
#define GL_ELEMENT_ARRAY_POINTER_ATI 0x876A
#endif
#ifndef GL_QUAD_MESH_SUN
#define GL_QUAD_MESH_SUN 0x8614
#endif
#ifndef GL_TRIANGLE_MESH_SUN
#define GL_TRIANGLE_MESH_SUN 0x8615
#endif
#ifndef GL_SLICE_ACCUM_SUN
#define GL_SLICE_ACCUM_SUN 0x85CC
#endif
#ifndef GL_MULTISAMPLE_FILTER_HINT_NV
#define GL_MULTISAMPLE_FILTER_HINT_NV 0x8534
#endif
#ifndef GL_DEPTH_CLAMP_NV
#define GL_DEPTH_CLAMP_NV 0x864F
#endif
#ifndef GL_PIXEL_COUNTER_BITS_NV
#define GL_PIXEL_COUNTER_BITS_NV 0x8864
#endif
#ifndef GL_CURRENT_OCCLUSION_QUERY_ID_NV
#define GL_CURRENT_OCCLUSION_QUERY_ID_NV 0x8865
#endif
#ifndef GL_PIXEL_COUNT_NV
#define GL_PIXEL_COUNT_NV 0x8866
#endif
#ifndef GL_PIXEL_COUNT_AVAILABLE_NV
#define GL_PIXEL_COUNT_AVAILABLE_NV 0x8867
#endif
#ifndef GL_POINT_SPRITE_NV
#define GL_POINT_SPRITE_NV 0x8861
#endif
#ifndef GL_COORD_REPLACE_NV
#define GL_COORD_REPLACE_NV 0x8862
#endif
#ifndef GL_POINT_SPRITE_R_MODE_NV
#define GL_POINT_SPRITE_R_MODE_NV 0x8863
#endif
#ifndef GL_OFFSET_PROJECTIVE_TEXTURE_2D_NV
#define GL_OFFSET_PROJECTIVE_TEXTURE_2D_NV 0x8850
#endif
#ifndef GL_OFFSET_PROJECTIVE_TEXTURE_2D_SCALE_NV
#define GL_OFFSET_PROJECTIVE_TEXTURE_2D_SCALE_NV 0x8851
#endif
#ifndef GL_OFFSET_PROJECTIVE_TEXTURE_RECTANGLE_NV
#define GL_OFFSET_PROJECTIVE_TEXTURE_RECTANGLE_NV 0x8852
#endif
#ifndef GL_OFFSET_PROJECTIVE_TEXTURE_RECTANGLE_SCALE_NV
#define GL_OFFSET_PROJECTIVE_TEXTURE_RECTANGLE_SCALE_NV 0x8853
#endif
#ifndef GL_OFFSET_HILO_TEXTURE_2D_NV
#define GL_OFFSET_HILO_TEXTURE_2D_NV 0x8854
#endif
#ifndef GL_OFFSET_HILO_TEXTURE_RECTANGLE_NV
#define GL_OFFSET_HILO_TEXTURE_RECTANGLE_NV 0x8855
#endif
#ifndef GL_OFFSET_HILO_PROJECTIVE_TEXTURE_2D_NV
#define GL_OFFSET_HILO_PROJECTIVE_TEXTURE_2D_NV 0x8856
#endif
#ifndef GL_OFFSET_HILO_PROJECTIVE_TEXTURE_RECTANGLE_NV
#define GL_OFFSET_HILO_PROJECTIVE_TEXTURE_RECTANGLE_NV 0x8857
#endif
#ifndef GL_DEPENDENT_HILO_TEXTURE_2D_NV
#define GL_DEPENDENT_HILO_TEXTURE_2D_NV 0x8858
#endif
#ifndef GL_DEPENDENT_RGB_TEXTURE_3D_NV
#define GL_DEPENDENT_RGB_TEXTURE_3D_NV 0x8859
#endif
#ifndef GL_DEPENDENT_RGB_TEXTURE_CUBE_MAP_NV
#define GL_DEPENDENT_RGB_TEXTURE_CUBE_MAP_NV 0x885A
#endif
#ifndef GL_DOT_PRODUCT_PASS_THROUGH_NV
#define GL_DOT_PRODUCT_PASS_THROUGH_NV 0x885B
#endif
#ifndef GL_DOT_PRODUCT_TEXTURE_1D_NV
#define GL_DOT_PRODUCT_TEXTURE_1D_NV 0x885C
#endif
#ifndef GL_DOT_PRODUCT_AFFINE_DEPTH_REPLACE_NV
#define GL_DOT_PRODUCT_AFFINE_DEPTH_REPLACE_NV 0x885D
#endif
#ifndef GL_HILO8_NV
#define GL_HILO8_NV 0x885E
#endif
#ifndef GL_SIGNED_HILO8_NV
#define GL_SIGNED_HILO8_NV 0x885F
#endif
#ifndef GL_FORCE_BLUE_TO_ONE_NV
#define GL_FORCE_BLUE_TO_ONE_NV 0x8860
#endif
#ifndef GL_STENCIL_TEST_TWO_SIDE_EXT
#define GL_STENCIL_TEST_TWO_SIDE_EXT 0x8910
#endif
#ifndef GL_ACTIVE_STENCIL_FACE_EXT
#define GL_ACTIVE_STENCIL_FACE_EXT 0x8911
#endif
#ifndef GL_TEXT_FRAGMENT_SHADER_ATI
#define GL_TEXT_FRAGMENT_SHADER_ATI 0x8200
#endif
#ifndef GL_UNPACK_CLIENT_STORAGE_APPLE
#define GL_UNPACK_CLIENT_STORAGE_APPLE 0x85B2
#endif
#ifndef GL_ELEMENT_ARRAY_APPLE
#define GL_ELEMENT_ARRAY_APPLE 0x8768
#endif
#ifndef GL_ELEMENT_ARRAY_TYPE_APPLE
#define GL_ELEMENT_ARRAY_TYPE_APPLE 0x8769
#endif
#ifndef GL_ELEMENT_ARRAY_POINTER_APPLE
#define GL_ELEMENT_ARRAY_POINTER_APPLE 0x876A
#endif
#ifndef GL_DRAW_PIXELS_APPLE
#define GL_DRAW_PIXELS_APPLE 0x8A0A
#endif
#ifndef GL_FENCE_APPLE
#define GL_FENCE_APPLE 0x8A0B
#endif
#ifndef GL_VERTEX_ARRAY_BINDING_APPLE
#define GL_VERTEX_ARRAY_BINDING_APPLE 0x85B5
#endif
#ifndef GL_VERTEX_ARRAY_RANGE_APPLE
#define GL_VERTEX_ARRAY_RANGE_APPLE 0x851D
#endif
#ifndef GL_VERTEX_ARRAY_RANGE_LENGTH_APPLE
#define GL_VERTEX_ARRAY_RANGE_LENGTH_APPLE 0x851E
#endif
#ifndef GL_VERTEX_ARRAY_STORAGE_HINT_APPLE
#define GL_VERTEX_ARRAY_STORAGE_HINT_APPLE 0x851F
#endif
#ifndef GL_VERTEX_ARRAY_RANGE_POINTER_APPLE
#define GL_VERTEX_ARRAY_RANGE_POINTER_APPLE 0x8521
#endif
#ifndef GL_STORAGE_CACHED_APPLE
#define GL_STORAGE_CACHED_APPLE 0x85BE
#endif
#ifndef GL_STORAGE_SHARED_APPLE
#define GL_STORAGE_SHARED_APPLE 0x85BF
#endif
#ifndef GL_YCBCR_422_APPLE
#define GL_YCBCR_422_APPLE 0x85B9
#endif
#ifndef GL_UNSIGNED_SHORT_8_8_APPLE
#define GL_UNSIGNED_SHORT_8_8_APPLE 0x85BA
#endif
#ifndef GL_UNSIGNED_SHORT_8_8_REV_APPLE
#define GL_UNSIGNED_SHORT_8_8_REV_APPLE 0x85BB
#endif
#ifndef GL_RGB_S3TC
#define GL_RGB_S3TC 0x83A0
#endif
#ifndef GL_RGB4_S3TC
#define GL_RGB4_S3TC 0x83A1
#endif
#ifndef GL_RGBA_S3TC
#define GL_RGBA_S3TC 0x83A2
#endif
#ifndef GL_RGBA4_S3TC
#define GL_RGBA4_S3TC 0x83A3
#endif
#ifndef GL_MAX_DRAW_BUFFERS_ATI
#define GL_MAX_DRAW_BUFFERS_ATI 0x8824
#endif
#ifndef GL_DRAW_BUFFER0_ATI
#define GL_DRAW_BUFFER0_ATI 0x8825
#endif
#ifndef GL_DRAW_BUFFER1_ATI
#define GL_DRAW_BUFFER1_ATI 0x8826
#endif
#ifndef GL_DRAW_BUFFER2_ATI
#define GL_DRAW_BUFFER2_ATI 0x8827
#endif
#ifndef GL_DRAW_BUFFER3_ATI
#define GL_DRAW_BUFFER3_ATI 0x8828
#endif
#ifndef GL_DRAW_BUFFER4_ATI
#define GL_DRAW_BUFFER4_ATI 0x8829
#endif
#ifndef GL_DRAW_BUFFER5_ATI
#define GL_DRAW_BUFFER5_ATI 0x882A
#endif
#ifndef GL_DRAW_BUFFER6_ATI
#define GL_DRAW_BUFFER6_ATI 0x882B
#endif
#ifndef GL_DRAW_BUFFER7_ATI
#define GL_DRAW_BUFFER7_ATI 0x882C
#endif
#ifndef GL_DRAW_BUFFER8_ATI
#define GL_DRAW_BUFFER8_ATI 0x882D
#endif
#ifndef GL_DRAW_BUFFER9_ATI
#define GL_DRAW_BUFFER9_ATI 0x882E
#endif
#ifndef GL_DRAW_BUFFER10_ATI
#define GL_DRAW_BUFFER10_ATI 0x882F
#endif
#ifndef GL_DRAW_BUFFER11_ATI
#define GL_DRAW_BUFFER11_ATI 0x8830
#endif
#ifndef GL_DRAW_BUFFER12_ATI
#define GL_DRAW_BUFFER12_ATI 0x8831
#endif
#ifndef GL_DRAW_BUFFER13_ATI
#define GL_DRAW_BUFFER13_ATI 0x8832
#endif
#ifndef GL_DRAW_BUFFER14_ATI
#define GL_DRAW_BUFFER14_ATI 0x8833
#endif
#ifndef GL_DRAW_BUFFER15_ATI
#define GL_DRAW_BUFFER15_ATI 0x8834
#endif
#ifndef GL_TYPE_RGBA_FLOAT_ATI
#define GL_TYPE_RGBA_FLOAT_ATI 0x8820
#endif
#ifndef GL_COLOR_CLEAR_UNCLAMPED_VALUE_ATI
#define GL_COLOR_CLEAR_UNCLAMPED_VALUE_ATI 0x8835
#endif
#ifndef GL_MODULATE_ADD_ATI
#define GL_MODULATE_ADD_ATI 0x8744
#endif
#ifndef GL_MODULATE_SIGNED_ADD_ATI
#define GL_MODULATE_SIGNED_ADD_ATI 0x8745
#endif
#ifndef GL_MODULATE_SUBTRACT_ATI
#define GL_MODULATE_SUBTRACT_ATI 0x8746
#endif
#ifndef GL_RGBA_FLOAT32_ATI
#define GL_RGBA_FLOAT32_ATI 0x8814
#endif
#ifndef GL_RGB_FLOAT32_ATI
#define GL_RGB_FLOAT32_ATI 0x8815
#endif
#ifndef GL_ALPHA_FLOAT32_ATI
#define GL_ALPHA_FLOAT32_ATI 0x8816
#endif
#ifndef GL_INTENSITY_FLOAT32_ATI
#define GL_INTENSITY_FLOAT32_ATI 0x8817
#endif
#ifndef GL_LUMINANCE_FLOAT32_ATI
#define GL_LUMINANCE_FLOAT32_ATI 0x8818
#endif
#ifndef GL_LUMINANCE_ALPHA_FLOAT32_ATI
#define GL_LUMINANCE_ALPHA_FLOAT32_ATI 0x8819
#endif
#ifndef GL_RGBA_FLOAT16_ATI
#define GL_RGBA_FLOAT16_ATI 0x881A
#endif
#ifndef GL_RGB_FLOAT16_ATI
#define GL_RGB_FLOAT16_ATI 0x881B
#endif
#ifndef GL_ALPHA_FLOAT16_ATI
#define GL_ALPHA_FLOAT16_ATI 0x881C
#endif
#ifndef GL_INTENSITY_FLOAT16_ATI
#define GL_INTENSITY_FLOAT16_ATI 0x881D
#endif
#ifndef GL_LUMINANCE_FLOAT16_ATI
#define GL_LUMINANCE_FLOAT16_ATI 0x881E
#endif
#ifndef GL_LUMINANCE_ALPHA_FLOAT16_ATI
#define GL_LUMINANCE_ALPHA_FLOAT16_ATI 0x881F
#endif
#ifndef GL_FLOAT_R_NV
#define GL_FLOAT_R_NV 0x8880
#endif
#ifndef GL_FLOAT_RG_NV
#define GL_FLOAT_RG_NV 0x8881
#endif
#ifndef GL_FLOAT_RGB_NV
#define GL_FLOAT_RGB_NV 0x8882
#endif
#ifndef GL_FLOAT_RGBA_NV
#define GL_FLOAT_RGBA_NV 0x8883
#endif
#ifndef GL_FLOAT_R16_NV
#define GL_FLOAT_R16_NV 0x8884
#endif
#ifndef GL_FLOAT_R32_NV
#define GL_FLOAT_R32_NV 0x8885
#endif
#ifndef GL_FLOAT_RG16_NV
#define GL_FLOAT_RG16_NV 0x8886
#endif
#ifndef GL_FLOAT_RG32_NV
#define GL_FLOAT_RG32_NV 0x8887
#endif
#ifndef GL_FLOAT_RGB16_NV
#define GL_FLOAT_RGB16_NV 0x8888
#endif
#ifndef GL_FLOAT_RGB32_NV
#define GL_FLOAT_RGB32_NV 0x8889
#endif
#ifndef GL_FLOAT_RGBA16_NV
#define GL_FLOAT_RGBA16_NV 0x888A
#endif
#ifndef GL_FLOAT_RGBA32_NV
#define GL_FLOAT_RGBA32_NV 0x888B
#endif
#ifndef GL_TEXTURE_FLOAT_COMPONENTS_NV
#define GL_TEXTURE_FLOAT_COMPONENTS_NV 0x888C
#endif
#ifndef GL_FLOAT_CLEAR_COLOR_VALUE_NV
#define GL_FLOAT_CLEAR_COLOR_VALUE_NV 0x888D
#endif
#ifndef GL_FLOAT_RGBA_MODE_NV
#define GL_FLOAT_RGBA_MODE_NV 0x888E
#endif
#ifndef GL_MAX_FRAGMENT_PROGRAM_LOCAL_PARAMETERS_NV
#define GL_MAX_FRAGMENT_PROGRAM_LOCAL_PARAMETERS_NV 0x8868
#endif
#ifndef GL_FRAGMENT_PROGRAM_NV
#define GL_FRAGMENT_PROGRAM_NV 0x8870
#endif
#ifndef GL_MAX_TEXTURE_COORDS_NV
#define GL_MAX_TEXTURE_COORDS_NV 0x8871
#endif
#ifndef GL_MAX_TEXTURE_IMAGE_UNITS_NV
#define GL_MAX_TEXTURE_IMAGE_UNITS_NV 0x8872
#endif
#ifndef GL_FRAGMENT_PROGRAM_BINDING_NV
#define GL_FRAGMENT_PROGRAM_BINDING_NV 0x8873
#endif
#ifndef GL_PROGRAM_ERROR_STRING_NV
#define GL_PROGRAM_ERROR_STRING_NV 0x8874
#endif
#ifndef GL_HALF_FLOAT_NV
#define GL_HALF_FLOAT_NV 0x140B
#endif
#ifndef GL_WRITE_PIXEL_DATA_RANGE_NV
#define GL_WRITE_PIXEL_DATA_RANGE_NV 0x8878
#endif
#ifndef GL_READ_PIXEL_DATA_RANGE_NV
#define GL_READ_PIXEL_DATA_RANGE_NV 0x8879
#endif
#ifndef GL_WRITE_PIXEL_DATA_RANGE_LENGTH_NV
#define GL_WRITE_PIXEL_DATA_RANGE_LENGTH_NV 0x887A
#endif
#ifndef GL_READ_PIXEL_DATA_RANGE_LENGTH_NV
#define GL_READ_PIXEL_DATA_RANGE_LENGTH_NV 0x887B
#endif
#ifndef GL_WRITE_PIXEL_DATA_RANGE_POINTER_NV
#define GL_WRITE_PIXEL_DATA_RANGE_POINTER_NV 0x887C
#endif
#ifndef GL_READ_PIXEL_DATA_RANGE_POINTER_NV
#define GL_READ_PIXEL_DATA_RANGE_POINTER_NV 0x887D
#endif
#ifndef GL_PRIMITIVE_RESTART_NV
#define GL_PRIMITIVE_RESTART_NV 0x8558
#endif
#ifndef GL_PRIMITIVE_RESTART_INDEX_NV
#define GL_PRIMITIVE_RESTART_INDEX_NV 0x8559
#endif
#ifndef GL_TEXTURE_UNSIGNED_REMAP_MODE_NV
#define GL_TEXTURE_UNSIGNED_REMAP_MODE_NV 0x888F
#endif
#ifndef GL_STENCIL_BACK_FUNC_ATI
#define GL_STENCIL_BACK_FUNC_ATI 0x8800
#endif
#ifndef GL_STENCIL_BACK_FAIL_ATI
#define GL_STENCIL_BACK_FAIL_ATI 0x8801
#endif
#ifndef GL_STENCIL_BACK_PASS_DEPTH_FAIL_ATI
#define GL_STENCIL_BACK_PASS_DEPTH_FAIL_ATI 0x8802
#endif
#ifndef GL_STENCIL_BACK_PASS_DEPTH_PASS_ATI
#define GL_STENCIL_BACK_PASS_DEPTH_PASS_ATI 0x8803
#endif
#ifndef GL_IMPLEMENTATION_COLOR_READ_TYPE_OES
#define GL_IMPLEMENTATION_COLOR_READ_TYPE_OES 0x8B9A
#endif
#ifndef GL_IMPLEMENTATION_COLOR_READ_FORMAT_OES
#define GL_IMPLEMENTATION_COLOR_READ_FORMAT_OES 0x8B9B
#endif
#ifndef GL_DEPTH_BOUNDS_TEST_EXT
#define GL_DEPTH_BOUNDS_TEST_EXT 0x8890
#endif
#ifndef GL_DEPTH_BOUNDS_EXT
#define GL_DEPTH_BOUNDS_EXT 0x8891
#endif
#ifndef GL_MIRROR_CLAMP_EXT
#define GL_MIRROR_CLAMP_EXT 0x8742
#endif
#ifndef GL_MIRROR_CLAMP_TO_EDGE_EXT
#define GL_MIRROR_CLAMP_TO_EDGE_EXT 0x8743
#endif
#ifndef GL_MIRROR_CLAMP_TO_BORDER_EXT
#define GL_MIRROR_CLAMP_TO_BORDER_EXT 0x8912
#endif
#ifndef GL_BLEND_EQUATION_RGB_EXT
#define GL_BLEND_EQUATION_RGB_EXT GL_BLEND_EQUATION
#endif
#ifndef GL_BLEND_EQUATION_ALPHA_EXT
#define GL_BLEND_EQUATION_ALPHA_EXT 0x883D
#endif
#ifndef GL_PACK_INVERT_MESA
#define GL_PACK_INVERT_MESA 0x8758
#endif
#ifndef GL_UNSIGNED_SHORT_8_8_MESA
#define GL_UNSIGNED_SHORT_8_8_MESA 0x85BA
#endif
#ifndef GL_UNSIGNED_SHORT_8_8_REV_MESA
#define GL_UNSIGNED_SHORT_8_8_REV_MESA 0x85BB
#endif
#ifndef GL_YCBCR_MESA
#define GL_YCBCR_MESA 0x8757
#endif
#ifndef GL_PIXEL_PACK_BUFFER_EXT
#define GL_PIXEL_PACK_BUFFER_EXT 0x88EB
#endif
#ifndef GL_PIXEL_UNPACK_BUFFER_EXT
#define GL_PIXEL_UNPACK_BUFFER_EXT 0x88EC
#endif
#ifndef GL_PIXEL_PACK_BUFFER_BINDING_EXT
#define GL_PIXEL_PACK_BUFFER_BINDING_EXT 0x88ED
#endif
#ifndef GL_PIXEL_UNPACK_BUFFER_BINDING_EXT
#define GL_PIXEL_UNPACK_BUFFER_BINDING_EXT 0x88EF
#endif
#ifndef GL_MAX_PROGRAM_EXEC_INSTRUCTIONS_NV
#define GL_MAX_PROGRAM_EXEC_INSTRUCTIONS_NV 0x88F4
#endif
#ifndef GL_MAX_PROGRAM_CALL_DEPTH_NV
#define GL_MAX_PROGRAM_CALL_DEPTH_NV 0x88F5
#endif
#ifndef GL_MAX_PROGRAM_IF_DEPTH_NV
#define GL_MAX_PROGRAM_IF_DEPTH_NV 0x88F6
#endif
#ifndef GL_MAX_PROGRAM_LOOP_DEPTH_NV
#define GL_MAX_PROGRAM_LOOP_DEPTH_NV 0x88F7
#endif
#ifndef GL_MAX_PROGRAM_LOOP_COUNT_NV
#define GL_MAX_PROGRAM_LOOP_COUNT_NV 0x88F8
#endif
#ifndef GL_INVALID_FRAMEBUFFER_OPERATION_EXT
#define GL_INVALID_FRAMEBUFFER_OPERATION_EXT 0x0506
#endif
#ifndef GL_MAX_RENDERBUFFER_SIZE_EXT
#define GL_MAX_RENDERBUFFER_SIZE_EXT 0x84E8
#endif
#ifndef GL_FRAMEBUFFER_BINDING_EXT
#define GL_FRAMEBUFFER_BINDING_EXT 0x8CA6
#endif
#ifndef GL_RENDERBUFFER_BINDING_EXT
#define GL_RENDERBUFFER_BINDING_EXT 0x8CA7
#endif
#ifndef GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_EXT
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_EXT 0x8CD0
#endif
#ifndef GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME_EXT
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME_EXT 0x8CD1
#endif
#ifndef GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL_EXT
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL_EXT 0x8CD2
#endif
#ifndef GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE_EXT
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE_EXT 0x8CD3
#endif
#ifndef GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_3D_ZOFFSET_EXT
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_3D_ZOFFSET_EXT 0x8CD4
#endif
#ifndef GL_FRAMEBUFFER_COMPLETE_EXT
#define GL_FRAMEBUFFER_COMPLETE_EXT 0x8CD5
#endif
#ifndef GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT 0x8CD6
#endif
#ifndef GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT 0x8CD7
#endif
#ifndef GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT
#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT 0x8CD9
#endif
#ifndef GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT
#define GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT 0x8CDA
#endif
#ifndef GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT
#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT 0x8CDB
#endif
#ifndef GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT
#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT 0x8CDC
#endif
#ifndef GL_FRAMEBUFFER_UNSUPPORTED_EXT
#define GL_FRAMEBUFFER_UNSUPPORTED_EXT 0x8CDD
#endif
#ifndef GL_MAX_COLOR_ATTACHMENTS_EXT
#define GL_MAX_COLOR_ATTACHMENTS_EXT 0x8CDF
#endif
#ifndef GL_COLOR_ATTACHMENT0_EXT
#define GL_COLOR_ATTACHMENT0_EXT 0x8CE0
#endif
#ifndef GL_COLOR_ATTACHMENT1_EXT
#define GL_COLOR_ATTACHMENT1_EXT 0x8CE1
#endif
#ifndef GL_COLOR_ATTACHMENT2_EXT
#define GL_COLOR_ATTACHMENT2_EXT 0x8CE2
#endif
#ifndef GL_COLOR_ATTACHMENT3_EXT
#define GL_COLOR_ATTACHMENT3_EXT 0x8CE3
#endif
#ifndef GL_COLOR_ATTACHMENT4_EXT
#define GL_COLOR_ATTACHMENT4_EXT 0x8CE4
#endif
#ifndef GL_COLOR_ATTACHMENT5_EXT
#define GL_COLOR_ATTACHMENT5_EXT 0x8CE5
#endif
#ifndef GL_COLOR_ATTACHMENT6_EXT
#define GL_COLOR_ATTACHMENT6_EXT 0x8CE6
#endif
#ifndef GL_COLOR_ATTACHMENT7_EXT
#define GL_COLOR_ATTACHMENT7_EXT 0x8CE7
#endif
#ifndef GL_COLOR_ATTACHMENT8_EXT
#define GL_COLOR_ATTACHMENT8_EXT 0x8CE8
#endif
#ifndef GL_COLOR_ATTACHMENT9_EXT
#define GL_COLOR_ATTACHMENT9_EXT 0x8CE9
#endif
#ifndef GL_COLOR_ATTACHMENT10_EXT
#define GL_COLOR_ATTACHMENT10_EXT 0x8CEA
#endif
#ifndef GL_COLOR_ATTACHMENT11_EXT
#define GL_COLOR_ATTACHMENT11_EXT 0x8CEB
#endif
#ifndef GL_COLOR_ATTACHMENT12_EXT
#define GL_COLOR_ATTACHMENT12_EXT 0x8CEC
#endif
#ifndef GL_COLOR_ATTACHMENT13_EXT
#define GL_COLOR_ATTACHMENT13_EXT 0x8CED
#endif
#ifndef GL_COLOR_ATTACHMENT14_EXT
#define GL_COLOR_ATTACHMENT14_EXT 0x8CEE
#endif
#ifndef GL_COLOR_ATTACHMENT15_EXT
#define GL_COLOR_ATTACHMENT15_EXT 0x8CEF
#endif
#ifndef GL_DEPTH_ATTACHMENT_EXT
#define GL_DEPTH_ATTACHMENT_EXT 0x8D00
#endif
#ifndef GL_STENCIL_ATTACHMENT_EXT
#define GL_STENCIL_ATTACHMENT_EXT 0x8D20
#endif
#ifndef GL_FRAMEBUFFER_EXT
#define GL_FRAMEBUFFER_EXT 0x8D40
#endif
#ifndef GL_RENDERBUFFER_EXT
#define GL_RENDERBUFFER_EXT 0x8D41
#endif
#ifndef GL_RENDERBUFFER_WIDTH_EXT
#define GL_RENDERBUFFER_WIDTH_EXT 0x8D42
#endif
#ifndef GL_RENDERBUFFER_HEIGHT_EXT
#define GL_RENDERBUFFER_HEIGHT_EXT 0x8D43
#endif
#ifndef GL_RENDERBUFFER_INTERNAL_FORMAT_EXT
#define GL_RENDERBUFFER_INTERNAL_FORMAT_EXT 0x8D44
#endif
#ifndef GL_STENCIL_INDEX1_EXT
#define GL_STENCIL_INDEX1_EXT 0x8D46
#endif
#ifndef GL_STENCIL_INDEX4_EXT
#define GL_STENCIL_INDEX4_EXT 0x8D47
#endif
#ifndef GL_STENCIL_INDEX8_EXT
#define GL_STENCIL_INDEX8_EXT 0x8D48
#endif
#ifndef GL_STENCIL_INDEX16_EXT
#define GL_STENCIL_INDEX16_EXT 0x8D49
#endif
#ifndef GL_RENDERBUFFER_RED_SIZE_EXT
#define GL_RENDERBUFFER_RED_SIZE_EXT 0x8D50
#endif
#ifndef GL_RENDERBUFFER_GREEN_SIZE_EXT
#define GL_RENDERBUFFER_GREEN_SIZE_EXT 0x8D51
#endif
#ifndef GL_RENDERBUFFER_BLUE_SIZE_EXT
#define GL_RENDERBUFFER_BLUE_SIZE_EXT 0x8D52
#endif
#ifndef GL_RENDERBUFFER_ALPHA_SIZE_EXT
#define GL_RENDERBUFFER_ALPHA_SIZE_EXT 0x8D53
#endif
#ifndef GL_RENDERBUFFER_DEPTH_SIZE_EXT
#define GL_RENDERBUFFER_DEPTH_SIZE_EXT 0x8D54
#endif
#ifndef GL_RENDERBUFFER_STENCIL_SIZE_EXT
#define GL_RENDERBUFFER_STENCIL_SIZE_EXT 0x8D55
#endif
#ifndef GL_DEPTH_STENCIL_EXT
#define GL_DEPTH_STENCIL_EXT 0x84F9
#endif
#ifndef GL_UNSIGNED_INT_24_8_EXT
#define GL_UNSIGNED_INT_24_8_EXT 0x84FA
#endif
#ifndef GL_DEPTH24_STENCIL8_EXT
#define GL_DEPTH24_STENCIL8_EXT 0x88F0
#endif
#ifndef GL_TEXTURE_STENCIL_SIZE_EXT
#define GL_TEXTURE_STENCIL_SIZE_EXT 0x88F1
#endif
#ifndef GL_STENCIL_TAG_BITS_EXT
#define GL_STENCIL_TAG_BITS_EXT 0x88F2
#endif
#ifndef GL_STENCIL_CLEAR_TAG_VALUE_EXT
#define GL_STENCIL_CLEAR_TAG_VALUE_EXT 0x88F3
#endif
#ifndef GL_SRGB_EXT
#define GL_SRGB_EXT 0x8C40
#endif
#ifndef GL_SRGB8_EXT
#define GL_SRGB8_EXT 0x8C41
#endif
#ifndef GL_SRGB_ALPHA_EXT
#define GL_SRGB_ALPHA_EXT 0x8C42
#endif
#ifndef GL_SRGB8_ALPHA8_EXT
#define GL_SRGB8_ALPHA8_EXT 0x8C43
#endif
#ifndef GL_SLUMINANCE_ALPHA_EXT
#define GL_SLUMINANCE_ALPHA_EXT 0x8C44
#endif
#ifndef GL_SLUMINANCE8_ALPHA8_EXT
#define GL_SLUMINANCE8_ALPHA8_EXT 0x8C45
#endif
#ifndef GL_SLUMINANCE_EXT
#define GL_SLUMINANCE_EXT 0x8C46
#endif
#ifndef GL_SLUMINANCE8_EXT
#define GL_SLUMINANCE8_EXT 0x8C47
#endif
#ifndef GL_COMPRESSED_SRGB_EXT
#define GL_COMPRESSED_SRGB_EXT 0x8C48
#endif
#ifndef GL_COMPRESSED_SRGB_ALPHA_EXT
#define GL_COMPRESSED_SRGB_ALPHA_EXT 0x8C49
#endif
#ifndef GL_COMPRESSED_SLUMINANCE_EXT
#define GL_COMPRESSED_SLUMINANCE_EXT 0x8C4A
#endif
#ifndef GL_COMPRESSED_SLUMINANCE_ALPHA_EXT
#define GL_COMPRESSED_SLUMINANCE_ALPHA_EXT 0x8C4B
#endif
#ifndef GL_COMPRESSED_SRGB_S3TC_DXT1_EXT
#define GL_COMPRESSED_SRGB_S3TC_DXT1_EXT 0x8C4C
#endif
#ifndef GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT
#define GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT 0x8C4D
#endif
#ifndef GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT
#define GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT 0x8C4E
#endif
#ifndef GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT
#define GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT 0x8C4F
#endif
#ifndef GL_READ_FRAMEBUFFER_EXT
#define GL_READ_FRAMEBUFFER_EXT 0x8CA8
#endif
#ifndef GL_DRAW_FRAMEBUFFER_EXT
#define GL_DRAW_FRAMEBUFFER_EXT 0x8CA9
#endif
#ifndef GL_READ_FRAMEBUFFER_BINDING_EXT
#define GL_READ_FRAMEBUFFER_BINDING_EXT GL_FRAMEBUFFER_BINDING_EXT
#endif
#ifndef GL_DRAW_FRAMEBUFFER_BINDING_EXT
#define GL_DRAW_FRAMEBUFFER_BINDING_EXT 0x8CAA
#endif
#ifndef GL_RENDERBUFFER_SAMPLES_EXT
#define GL_RENDERBUFFER_SAMPLES_EXT 0x8CAB
#endif
#ifndef GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE_EXT
#define GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE_EXT 0x8D56
#endif
#ifndef GL_MAX_SAMPLES_EXT
#define GL_MAX_SAMPLES_EXT 0x8D57
#endif
#ifndef GL_TEXTURE_1D_STACK_MESAX
#define GL_TEXTURE_1D_STACK_MESAX 0x8759
#endif
#ifndef GL_TEXTURE_2D_STACK_MESAX
#define GL_TEXTURE_2D_STACK_MESAX 0x875A
#endif
#ifndef GL_PROXY_TEXTURE_1D_STACK_MESAX
#define GL_PROXY_TEXTURE_1D_STACK_MESAX 0x875B
#endif
#ifndef GL_PROXY_TEXTURE_2D_STACK_MESAX
#define GL_PROXY_TEXTURE_2D_STACK_MESAX 0x875C
#endif
#ifndef GL_TEXTURE_1D_STACK_BINDING_MESAX
#define GL_TEXTURE_1D_STACK_BINDING_MESAX 0x875D
#endif
#ifndef GL_TEXTURE_2D_STACK_BINDING_MESAX
#define GL_TEXTURE_2D_STACK_BINDING_MESAX 0x875E
#endif
#ifndef GL_TIME_ELAPSED_EXT
#define GL_TIME_ELAPSED_EXT 0x88BF
#endif
#ifndef GL_BUFFER_SERIALIZED_MODIFY_APPLE
#define GL_BUFFER_SERIALIZED_MODIFY_APPLE 0x8A12
#endif
#ifndef GL_BUFFER_FLUSHING_UNMAP_APPLE
#define GL_BUFFER_FLUSHING_UNMAP_APPLE 0x8A13
#endif
#ifndef GL_ALL_CLIENT_ATTRIB_BITS
#define GL_ALL_CLIENT_ATTRIB_BITS GL_CLIENT_ALL_ATTRIB_BITS
#endif


#endif /* _GLENUMS_H */

