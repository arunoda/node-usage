{

	'targets': [
		{
		    'target_name': 'proc',
			'type': 'static_library',
			'defines': [
				'clocksource=acpi_pm'
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
			'type': 'static_library',
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