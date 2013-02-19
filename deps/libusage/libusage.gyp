{
	'targets': [
		{
		    'target_name': 'proc',
			'type': 'shared_library',
			'defines': [

			],
			'include_dirs': [
				'proc/'
			],
			'sources': [
				'proc/sysinfo.c',
				'proc/readproc.c',
				'proc/escape.c',
				'proc/alloc.c',
				'proc/version.c',
				'proc/pwcache.c'
			],
			'cflags': [
				'-std=c99',
				'-fPIC'
			]
		},

		{
			'target_name': 'usage',
			'type': 'shared_library',
			'dependencies': [
				'proc'
			],
			'include_dirs': [
				'proc/'
			],
			'sources': [
				'usage.c'
			],
			'cflags': [
				'-std=c99',
				'-fPIC'
			]
		},

		{
			'target_name': 'test',
			'type': 'executable',
			'dependencies': [
				'usage'
			],
			'include_dirs': [
				'.',
				'proc/'
			],
			'sources': [
				'test.c'
			]
		}
	]
}