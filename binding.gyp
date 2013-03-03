{
	'targets': [
		{
			'target_name': 'sysinfo',
			'conditions': [
				['OS!="linux"', {
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