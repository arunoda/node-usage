{
	'targets': [
		{
			'target_name': 'sysinfo',
			'sources': [
				'src/binding.cpp'
			],
			'defines': [
				'OS="<(OS)"',
				'is_<(OS)'
			]
		}
	]
}