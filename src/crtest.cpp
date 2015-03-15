#include <gecode/int.hh>
#include <gecode/search.hh>
#include <gecode/minimodel.hh>
#include <gecode/set.hh>
#include <iostream>
#include <memory>

using namespace Gecode;
using namespace std;

// Space starts a CSP
class ConstraintRelationshipTest : public Space {
	protected:
		IntVar x, y, z;
		IntVar aux1, aux2, aux3; 
		// Domains 0-2
				
		// reified PVS values
		SetVar sc1;  // x + 1 = y
		SetVar sc2;  // z = y + 2
		SetVar sc3;  // x + y <= 3
		SetVar sol;
		SetVarArray softConstraints;
		
		// technical variables for reification (and inverted reify)
		BoolVar re1, re2, re3;
		BoolVar inv1, inv2, inv3;
	
	public:
		ConstraintRelationshipTest(void):  
		  re1(*this, 0, 1), inv1(*this, 0, 1), 
		  re2(*this, 0, 1), inv2(*this, 0, 1),
		  re3(*this, 0, 1), inv3(*this, 0, 1),
		  x(*this, 0,2), y(*this, 0,2), z(*this, 0,2),
		  aux1(*this, 1, 3),aux2(*this, 0, 5),aux3(*this, 0, 4), 
		  sc1(*this, IntSet::empty, IntSet(1, 3)),
		  sc2(*this, IntSet::empty, IntSet(1, 3)), 
		  sc3(*this, IntSet::empty, IntSet(1, 3)), 
		  sol(*this, IntSet::empty, IntSet(1, 3)),
		  softConstraints(*this, 3) {
			
			rel(*this, x + 1 == aux1); 
			rel(*this, aux1, IRT_EQ, y, re1);
		    
			rel(*this, y + 2 == aux2);
			rel(*this, z, IRT_EQ, aux2, re2);
			
			rel(*this, x + y == aux3);
			rel(*this, aux3, IRT_LE, 3, re3);
			
			rel(*this, inv1 != re1);
			rel(*this, inv2 != re2);
			rel(*this, inv3 != re3);
			
			dom(*this, sc1, SRT_EQ, IntSet::empty, re1);
			dom(*this, sc1, SRT_EQ, 1, inv1);
			
			dom(*this, sc2, SRT_EQ, IntSet::empty, re2);
			dom(*this, sc2, SRT_EQ, 2, inv2);
			
			dom(*this, sc3, SRT_EQ, IntSet::empty, re3);
			dom(*this, sc3, SRT_EQ, 3, inv3);
			
			softConstraints[0] = sc1;
			softConstraints[1] = sc2;
			softConstraints[2] = sc3;
			
			rel(*this, SOT_UNION, softConstraints, sol);
			//rel(*this, b, IRT_EQ, 1, reify);
			branch(*this, x, INT_VAL_MIN());
			branch(*this, y, INT_VAL_MIN());
			branch(*this, z, INT_VAL_MIN());
		}
		
		// now some technical support for search
		ConstraintRelationshipTest(bool share, ConstraintRelationshipTest& sp) : Space(share, sp) {
			x.update(*this, share, sp.x);
			y.update(*this, share, sp.y);
			z.update(*this, share, sp.z);
			
			re1.update(*this, share, sp.re1);
			re2.update(*this, share, sp.re2);
			re3.update(*this, share, sp.re3);

			aux1.update(*this, share, sp.aux1);
			aux2.update(*this, share, sp.aux2);
			aux3.update(*this, share, sp.aux3);
			
			inv1.update(*this, share, sp.inv1);
			inv2.update(*this, share, sp.inv2);
			inv3.update(*this, share, sp.inv3);
			
			sol.update(*this, share, sp.sol);
			softConstraints.update(*this, share, sp.softConstraints);
		}

		// issues new constraints in a branch and bound search upon finding a solution
		virtual void constrain(const Space& _b) {
			const ConstraintRelationshipTest& b = static_cast<const ConstraintRelationshipTest&>(_b);
			BoolVar bvar(*this, 0, 1);
			// the next found solution must !NOT! be a superset of the existing
			// violation sets
			rel(*this, sol, SRT_SUP, b.sol, bvar);
			rel(*this, ! bvar);
		}
		
		virtual Space* copy(bool share) {
			return new ConstraintRelationshipTest(share, *this);
		}
		
		void print(void) const {
			std::cout << " x " << x << 
			             " y " << y  << " z  " << z << " | " << sol << std::endl;
		}
		
};

int main(int argc, char** argv) {
	std::cout << "Welcome to the first Working eclipse GECODE Constraint Relationship test!" << std::endl;
	
	try {
		// Create the CSP itself first (the space)
		std::unique_ptr<ConstraintRelationshipTest> m(new ConstraintRelationshipTest);
				
		m->status();
		//DFS<ConstraintRelationshipTest> e(m.get()); // templated, nice!
		BAB<ConstraintRelationshipTest> e(m.get());
			
		while(ConstraintRelationshipTest* s = e.next()) {
			std::cout << "Next solution" << std::endl;
			s->print(); delete s;
		}
				
	} catch (Exception e) {
		std::cerr << "Gecode Exception: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}
