{
	'targets':
	[
		{
			'target_name': 'al',
			'include_dirs': [
				'<!(node -e "require(\'nan\')")',
				'deps/include',
			],
			"library_dirs": [
				'../deps/lib',
			],
			'libraries': [
				'-lallegro',
				'-lallegro_font',
				'-lallegro_color',
				'-lallegro_image',
			],
			"conditions:": [
				["OS=='mac'", {
					"libraries": [
						'-framework AppKit',
						'-framework IOKit',
						'-framework OpenGL',
					]
				}],
			],
			'cflags_cc': [
				'-std=c++11',
				'-fexceptions',
			],
			'msvs_settings': {
				'VCCLCompilerTool': {
					'ExceptionHandling': 1,
				},
			},
			'xcode_settings': {
				'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
			},
			'sources': [
				'hello.cc',
				'allegro.cc',
				'deps/include/v8pp/class.cpp',
				'deps/include/v8pp/context.cpp',
				'deps/include/v8pp/convert.cpp',
				'deps/include/v8pp/json.cpp',
				'deps/include/v8pp/throw_ex.cpp',
				'deps/include/v8pp/version.cpp,'
			],
		}
	]
}
