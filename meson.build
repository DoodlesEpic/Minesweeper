project('Minesweeper', 'cpp',
  version : '1.3',
  default_options : ['warning_level=3', 'cpp_std=c++17'])

wx_dep = dependency(
  'wxwidgets', version : '>=3.0.0', modules : ['core', 'base'], static : true,
)

executable('Minesweeper',
           'App.cpp',
           'MainGUI.cpp',
           dependencies: wx_dep,
           install : true)
