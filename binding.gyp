{
	"targets":
	[
		{
			"target_name":"eigenface",
			"sources":
			[
        "initializer.cpp",
				"eigenface.cpp"
			],
      "include_dirs": 
      [
      "/usr/include/boost"
      ],
      "libraries":[
       "-lboost_system", "-lboost_filesystem",
       "<!@(pkg-config --libs opencv)"
      ],
      "cflags_cc":
      [
        "-std=c++11",
        "-fexceptions",
        "`pkg-config --cflags opencv`"
      ],
      "cflags_cc!":
      [
        "-fno-rtti", "-fno-exceptions"
      ]

		}
	]
	
}
