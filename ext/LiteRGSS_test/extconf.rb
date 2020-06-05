require_relative 'mkmf_unit_test'
require 'fileutils'

have_library('sfml-graphics')
have_library('sfml-window')
have_library('sfml-system')

current_dir_path = File.expand_path(File.dirname(__FILE__)) 
CONFIG["codedir"] = current_dir_path + '/../LiteRGSS'

# add include path to the internal folder
# $(srcdir) is a root folder, where "extconf.rb" is stored
$INCFLAGS << " -I/usr/include/LiteCGSS/"

$CXXFLAGS += " -std=c++17 "

create_makefile("LiteRGSS_test", nil, 'LiteRGSS')
