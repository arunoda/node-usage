{
	'targets': [
		{
			'target_name': 'sysinfo',
			'conditions': [
				['OS=="linux"', {
					'include_dirs': [
						'deps/procps/proc/',
					],
					'dependencies': [
						'deps/procps/binding.gyp:sysinfo'
					]
				}, {
					'defines': [
						'PROC_NOT_SUPPORTED'
					]
				}]
			],
			'sources': [
				'src/binding.cpp'
			],
		}
	]
}