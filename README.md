# Fermion Bag Approach for Hamiltonian Lattice Field Theories

This code is a generalization of the code originally developed by Dr. Emilie Huffman in her graduate work at Duke University. 

## Abstract:
Understanding the critical behavior near quantum critical points for strongly correlated 
quantum many-body systems remains intractable for the vast majority of
scenarios. Challenges involve determining if a quantum phase transition is first- or
second-order, and finding the critical exponents for second-order phase transitions.
Learning about where second-order phase transitions occur and determining their
critical exponents is particularly interesting, because each new second-order phase
transition defines a new quantum field theory.
<br>
Quantum Monte Carlo (QMC) methods are one class of techniques that, when
applicable, offer reliable ways to extract the nonperturbative physics near strongly
coupled quantum critical points. However, there are two formidable bottlenecks to
the applicability of QMC: (1) the sign problem and (2) algorithmic update inefficiencies. 
In this thesis, I overcome both these difficulties for a class of problems by
extending the fermion bag approach recently developed by Shailesh Chandrasekharan to the
Hamiltonian formalism and by demonstrating progress using the example
of a specific quantum system known as the t-V model, which exhibits a transition
from a semimetal to an insulator phase for a single flavor of four-component Dirac
fermions.
<br>
I adapt the fermion bag approach, which was originally developed in the context
of Lagrangian lattice field theories, to be applicable within the Hamiltonian formalism, 
and demonstrate its success in two ways: first, through solutions to new sign
problems, and second, through the development of new efficient QMC algorithms. In
addressing the first point, I present a solution to the sign problem for the t-V model.
While the t-V model is the simplest Gross-Neveu model of the chiral Ising universality 
class, the specter of the sign problem previously prevented its simulation with
QMC for 30 years, and my solution initiated the first QMC studies for this model.
The solution is then extended to many other Hamiltonian models within a class that
involves fermions interacting with quantum spins. Some of these models contain an
interesting quantum phase transition between a massless/semimetal phase to a 
massive/insulator phase in the so called Gross-Neveu universality class. Thus, the new
solutions to the sign problem allow for the use of the QMC method to study these
universality classes.
<br>
The second point is addressed through the construction of a Hamiltonian fermion
bag algorithm. The algorithm is then used to compute the critical exponents for the
second-order phase transition in the t-V model. By pushing the calculations to 
significantly larger lattice sizes than previous recent computations ($64^2$
sites versus $24^2$ sites), I am able to compute the critical exponents more reliably here compared to
earlier work. I show that the inclusion of these larger lattices causes a significant
shift in the values of the critical exponents that was not evident for the smaller lattices. 
This shift puts the critical exponent values in closer agreement with continuum
4 - $\Epsilon$ expansion calculations. The largest lattice sizes of $64^2$ at a comparably low
temperature are reachable due to efficiency gains from this Hamiltonian fermion bag
algorithm. The two independent critical exponents I find, which completely characterize 
the phase transition, are η = .51(3) and $\Nu$ = .89(1), compared to previous
work that had lower values for these exponents. The finite size scaling fit is excellent
with a χ$^2$ / DOF = .90, showing strong evidence for a second-order critical phase
transition, and hence a non-perturbative QFT can be defined at the critical point.
<br>
## Theoretical Development and Algorithm Description:
[Fermion Bag Approach for Hamiltonian Lattice Field Theories](https://arxiv.org/abs/1912.11776)
