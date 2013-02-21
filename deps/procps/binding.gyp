{

	'targets': [
		{
		    'target_name': 'sysinfo',
			'type': 'static_library',
			'defines': [
				
			],
			'include_dirs': [
				'proc/'
			],
			'sources': [
				'proc/sysinfo.c',
				'proc/version.c',
			],
			'cflags': [
				'-std=c99',
				'-fPIC',
				'-w'
			]
		},
	]
}