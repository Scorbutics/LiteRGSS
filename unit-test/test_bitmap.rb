# Load required library
require_relative "../lib/LiteRGSS"
include LiteRGSS
require "rspec"

# Start specification
RSpec.describe Graphics do
  it 'bitmap release' do
    Graphics.start
    bmp = Bitmap.new(32, 32)
    bmp = nil
    GC.start()
  end
end
