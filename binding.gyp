{
	'targets': [
		{
			'target_name': 'node_usage',
			'include_dirs': [
				'deps/libusage/proc/',
				'deps/libusage/'
			],
			'sources': [
				'src/binding.cpp'
			],
			'dependencies': [
				'deps/libusage/libusage.gyp:usage'
			]
		}
	]
}