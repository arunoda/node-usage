{
	'targets': [
		{
			'target_name': 'node_usage',
			'conditions': [
				['OS=="linux" and target_arch=="x64"', {
					'include_dirs': [
						'deps/libusage/proc/',
						'deps/libusage/'
					],
					'dependencies': [
						'deps/libusage/libusage.gyp:usage'
					]
				}, {
					'defines': [
						'USAGE_NOT_SUPPORTED'
					]
				}]
			],
			'sources': [
				'src/binding.cpp'
			],
		}
	]
}