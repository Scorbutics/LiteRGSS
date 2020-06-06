require_relative 'mkmf_unit_test'
require 'fileutils'

# add include path to the internal folder
# $(srcdir) is a root folder, where "extconf.rb" is stored
$INCFLAGS << " -I/usr/include/LiteCGSS/ -I$(srcdir)/../../ -I$(srcdir)/../../LiteCGSS/"
$LDFLAGS << " -L$(srcdir)/../../LiteCGSS_bin"

have_library('sfml-graphics')
have_library('sfml-window')
have_library('sfml-system')
have_library('LiteCGSS_engine')

current_dir_path = File.expand_path(File.dirname(__FILE__)) 
CONFIG["codedir"] = current_dir_path + '/../LiteRGSS'

$CXXFLAGS += " -std=c++17 "

create_makefile("LiteRGSS_test", nil, 'LiteRGSS')
