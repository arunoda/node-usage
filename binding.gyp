{
	'targets': [
		{
			'target_name': 'sysinfo',
			'sources': [
				'src/binding.cpp',
			],
			'linkflags': [
				'-Lbuild/cd Release/obj.target/sysinfo/src/'
			],
			'defines': [
				'OS="<(OS)"',
				'is_<(OS)'
			],
		}
	]
}