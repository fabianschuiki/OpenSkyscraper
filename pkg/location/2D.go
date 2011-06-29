package location

type Loc2D struct {
	X int // X Location on a floor, in 16 pixel increments.
	Y int // Y Location, == Floor number
}

func (l *Loc2D) Add(other *Loc2D) {
	l.X += other.X
	l.Y += other.Y
}

func (l *Loc2D) Sub(other *Loc2D) {
	l.X -= other.X
	l.Y -= other.Y
}