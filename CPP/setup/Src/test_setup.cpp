#include "setup/Output.hpp"
#include "test/Main.hpp"
#include <gsl/gsl_blas.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <numeric>

using namespace test;
using namespace cfl;
using namespace std;

namespace NSetup
{
void
function ()
{
  print ("Test cfl::Function");

  unsigned iS = 5;
  double dL = 2;
  double dR = dL + 1.0;
  double dX = 2.;

  Function uF ([] (double x) { return std::sin (x); }, dL, dR);
  Function uG ([] (double x) { return std::exp (x); }, dL, dR);
  uF += Function ([] (double x) { return std::exp (x); }, dL, dR);
  uF += Function ([] (double x) { return std::cos (x); });
  uG = Function ([] (double dY) { return std::pow (dY, 3.); },
                 [] (double dY) { return (dY > 0); });
  uF *= uG;

  printValues (uF, getArg (dL, dR, iS), "function f");

  uF += uG;
  uF -= uG;
  uF *= uG;
  uF /= uG;
  uF += dX;
  uF -= dX;
  uF *= dX;
  uF /= dX;
  uF = -uF;
  uF = -uF;
  uF = (uF * uG) / uG;
  uF = (uF / uG) * uG;
  uF = (uF - uG) + uG;
  uF = (uF + uG) - uG;
  uF = (uF + dX) - dX;
  uF = (dX + uF) - dX;
  uF = (uF * dX) / dX;
  uF = (uF / dX) * dX;
  uF = (uF + dX) - dX;
  uF = (uF - dX) + dX;
  uF = (dX + uF) - dX;
  uF = (dX - uF) * (-1) + dX;
  uF = (-1) * (dX - uF) + dX;

  uF = exp (log (uF));
  uF = log (exp (uF));
  uF = pow (sqrt (uF), 2);
  uF = cfl::apply (uF, [] (double dZ) { return dZ * dZ; });
  uF = apply (uF, uF, [] (double dX, double dY) { return dX + dY; });

  printValues (uF, getArg (dL, dR, iS),
               "again function f after manipulations");
};

void
gsl ()
{
  print ("Test of GSL and CBLAS");

  unsigned iN = 5;
  std::vector<double> uX (iN);
  std::vector<double> uY (iN);
  std::vector<double> uA (iN * iN);

  std::iota (uX.begin (), uX.end (), 0);
  std::iota (uY.begin (), uY.end (), uX.back () + 1);
  std::iota (uA.begin (), uA.end (), 0.);

  gsl_vector_const_view uXw
      = gsl_vector_const_view_array (uX.data (), uX.size ());
  gsl_vector_view uYw = gsl_vector_view_array (uY.data (), uY.size ());
  gsl_matrix_const_view uAw = gsl_matrix_const_view_array (uA.data (), iN, iN);

  std::vector<std::vector<double> > uPrint (2 + iN);
  std::vector<std::string> uNames (2 + iN, "");
  uPrint.front () = uX;
  uPrint[1] = uY;
  uNames.front () = "X";
  uNames[1] = "Y";
  uNames[2 + iN / 2] = "A";

  for (unsigned i = 0; i < iN; i++)
    {
      uPrint[i + 2] = std::vector<double> (uA.begin () + i * iN,
                                           uA.begin () + (i + 1) * iN);
    }

  printTable (uPrint, uNames, "vectors and matrix", iN, 3, iN);

  print (uX.begin (), uX.end (), "vector X");
  print (uY.begin (), uY.end (), "vector Y");

  double dZ;
  gsl_blas_ddot (&uXw.vector, &uYw.vector, &dZ);
  print (dZ, "scalar product of X and Y", true);

  gsl_blas_dgemv (CblasNoTrans, 1., &uAw.matrix, &uXw.vector, 1., &uYw.vector);
  print (uY.begin (), uY.end (), "product of matrix A and vector X");
}
}

std::function<void ()>
test_setup ()
{
  return [] () {
    print ("START OF SETUP");

    NSetup::function ();
    NSetup::gsl ();

    print ("END OF SETUP");
  };
}

int
main ()
{
  project (test_setup (), PROJECT_NAME, PROJECT_NAME, "setup");
}
