{
	'targets': [
		{
			'type': 'shared_library',
			'product_extension': 'node',
			'defines': [ 'BUILDING_NODE_EXTENSION' ],
			'target_name': 'usage',
			'include_dirs': [
				'deps/libusage',
				'deps/libusage/proc'
			],
			'sources': [
				'src/binding.cc'
			],
			'dependencies': [
				'deps/libusage/libusage.gyp:libusage'
			]
		}
	]
}