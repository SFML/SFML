class Vector2
  attr_accessor :x, :y
	
  def initialize( *args )
    if args.size == 0
      @x = 0
      @y = 0
    elsif args.size == 1
      copyFrom( args[0] )
    elsif args.size == 2
      Vector2.valid? args[0], args[1]
      
      @x = args[0]
      @y = args[1]
    else
      raise ArgumentError.new( "invalid argument list" )
    end
    
    @dataType = x.class
  end
  
  def copyFrom( source )
    unless source.is_a?( Array ) || source.is_a?( Vector2 )
      raise ArgumentError.new( "expected Array or Vector2" )
    end
    Vector2.valid? source[0], source[1]
    
    @x = source[0]
    @y = source[1]
  end
  
  def -@
    Vector2.new( -x, -y )
  end
  
  def +( right )
    Vector2.new( x + right.x, y + right.y )
  end
  
  def -( right )
    Vector2.new( x - right.x, y - right.y )
  end
  
  def *( right )
    Vector2.new( x * right.x, y * right.y )
  end
  
  def /( right )
    Vector2.new( x / right.x, y / right.y )
  end
  
  def ==( right )
    x == right.x && y == right.y
  end
  
  def []( index )
    if index == 0 || index == :x
      return x
    elsif index == 1 || index == :y
      return y
    end
    
    raise ArgumentError.new( "Expected index to be either 0..1 or :x and :y" )
  end
  
  def self.valid?( x, y )
    if x.class != y.class
      raise RuntimeError.new( "x and y must be of same type" )
    end
    
    if x.is_a?( Numeric ) == false
      raise RuntimeError.new( "x and y must be numeric!" )
    end
    
    true
  end
end
