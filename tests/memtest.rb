require "./lib/LiteRGSS.so"
include LiteRGSS
w = Config::ScreenWidth = 1280
h = Config::ScreenHeight = 720
Config::Title = "Test7 (memory)"
$LOAD_PATH << "."
Graphics.start
@v = Viewport.new(w, h)
@v.rect.width = w / 2
@v.rect.height = h / 2
#@v.tone.set(255, -255, 0, 0)

# placeholder for GC stats
b = {}


#> Graphic Test
bmp = Bitmap.new("cb.bmp")
t = Time.new
arr = Array.new(1500000) do #4000 -> down
  sp = Sprite.new(@v)
  sp.bitmap = bmp
  sp.zoom = rand / 2
  sp.set_position(rand(w), rand(h))
  0
end
Graphics.update
#Graphics.update #Why ?
@b = Sprite.new
t = Time.new
p @b.bitmap = Graphics.snap_to_bitmap
p Time.new - t
t = Time.new
Graphics.freeze

p Time.new - t
arr.clear
t = Time.new
@v = Viewport.new(w, h)
puts 'Before GC'
puts GC.stat(b)[:heap_live_slots]
GC.start
puts 'After GC'
puts GC.stat(b)[:heap_live_slots]
p Time.new - t
t = Time.new


@a = Sprite.new(@v)
@a.bitmap = bmp
@a.ox = 75
@a.oy = 60
@a.angle = 45
@a.x = w / 2
@a.y = h / 2
@a.opacity = 200
@running = true
i = 0
Graphics.instance_variable_set(:@on_close, proc { @running = false }) # The block returns false, it's important !
Graphics.transition(120)
t = Time.new
while @running
  Graphics.update
  @a.angle = (@a.angle + (10 + 10 * Math::cos(i * Math::PI / 20))) % 360
  i += 1
end
dt = (Time.new - t)
p 1 / (dt / i)
@b.bitmap = Graphics.snap_to_bitmap
Graphics.update

p "Waiting..."
sleep 6
p "2nd iteration"

arr = Array.new(1500000) do #4000 -> down
  sp = Sprite.new(@v)
  sp.bitmap = bmp
  sp.zoom = rand / 2
  sp.set_position(rand(w), rand(h))
  0
end

arr.clear
t = Time.new
@v.dispose
p "Before GC"
puts GC.stat(b)[:heap_live_slots]
GC.start
p "After GC"
puts GC.stat(b)[:heap_live_slots]
p Time.new - t

Graphics.stop
sleep 6