project('Minesweeper', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++17'])

wx_dep = dependency(
  'wxwidgets', version : '>=3.0.0', modules : ['std', 'stc'],
)

executable('Minesweeper',
           'App.cpp',
           'MainGUI.cpp',
           dependencies: wx_dep,
           install : true)
