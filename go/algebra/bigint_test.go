package algebra

import (
	"math/rand"
	"testing"
	"time"
)

func genVec(sz int) []uint64 {
	rng := rand.NewSource(time.Now().UnixNano() + 82)
	var v []uint64
	for i := 0; i < sz; i++ {
		v = append(v, uint64(rng.Int63()))
	}
	return v
}
func TestMul(t *testing.T) {
	u, v := genVec(1000), genVec(1000)
	x, y := Bigint{u}, Bigint{v}
	lhs, rhs := x.Mul(&y), y.Mul(&x)
	if !lhs.Compare(rhs) {
		t.Fatalf("Bigint mul is not commutative!\n")
	}
}
func TestAdd(t *testing.T) {
	u, v := genVec(10000), genVec(10000)
	x, y := Bigint{u}, Bigint{v}
	lhs, rhs := x.Add(&y), y.Add(&x)
	if !lhs.Compare(rhs) {
		t.Fatalf("Bigint add is not commutative!\n")
	}
}
func TestDistributiveLaw(t *testing.T) {
	u, v := genVec(100), genVec(100)
	x, y := Bigint{u}, Bigint{v}
	xpy, xx, xy := x.Add(&y), x.Mul(&x), x.Mul(&y)
	lhs, rhs := x.Mul(xpy), xx.Add(xy)
	if !lhs.Compare(rhs) {
		t.Fatalf("Bigint mul is not distributive over add!\n")
	}
}
