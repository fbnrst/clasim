#include <stdio.h>
#include "Python.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <exception>
#include <random>

#include "types.h"
#include "sample.h"
#include "cell.h"
#include "rand.h"

PyObject *vectorToList_Float(const std::vector<REAL> &data) {
  PyObject* listObj = PyList_New( data.size() );
  if (!listObj)
    throw std::logic_error("Unable to allocate memory for Python list");
  for (unsigned int i = 0; i < data.size(); i++) {
    PyObject *num = PyFloat_FromDouble((double)data[i]);
    if (!num) {
      Py_DECREF(listObj);
      throw std::logic_error("Unable to allocate memory for Python list");
    }
    PyList_SET_ITEM(listObj, i, num);
  }
  return listObj;
}

void PyOb_to_Vec(PyObject *obj, std::vector<REAL> &vec) {
  PyObject *iter = PyObject_GetIter(obj);
  if (!iter) {
    throw std::runtime_error("error not iterator");
    return;
  }

  while (true) {
    PyObject *next = PyIter_Next(iter);
    if (!next) {
      // nothing left in the iterator
      break;
    }

    if (!PyFloat_Check(next)) {
      throw std::runtime_error(
          "error, we were expecting a floating point value");
      return;
    }

    vec.push_back((REAL)PyFloat_AsDouble(next));
  }
  return;
}

static PyObject *run_wrapper(PyObject *self, PyObject *args) {
  std::setlocale(LC_ALL, "C");
  PyObject *pIn;
  t_InitParameter InitParameter;
  std::vector<REAL> Result;
  int mode, seed,nA,nC,mC;
  // parse arguments
  double GF, Tc, G1, S, G2M, sA, sP;
  if (!PyArg_ParseTuple(args, "iiiiOddddddi", &seed, &nA, &nC, &mC, &pIn, &GF,
                        &G1, &S, &G2M, &sA, &sP, &mode)) {
    return NULL;
  }

  InitParameter.nAnimals = nA;
  InitParameter.nCells = nC;
  InitParameter.mCells = mC;
  Tc = G1 + S + G2M;
  InitParameter.GF = (REAL)GF;
  InitParameter.Tc = (REAL)Tc;
  InitParameter.fS = (REAL)S / Tc;
  InitParameter.fG2M = (REAL)G2M / Tc;
  InitParameter.fG1 = (REAL)G1 / Tc;
  InitParameter.sAnimal = (REAL)sA;
  InitParameter.sPopulation = (REAL)sP;
  try {
    PyOb_to_Vec(pIn, InitParameter.KTimes);
  } catch (std::exception &e) {
    PyErr_SetString(PyExc_RuntimeError, e.what());
    return NULL;
  }
  static URNG generator;
  generator.seed(seed);
  InitParameter.urng = &generator;
  switch (mode) {
  // asymetric cell division, daughter has mother's parameter
  case 0: {
    r_lognorm rr(InitParameter.Tc, InitParameter.sPopulation);
    for (unsigned i = 0; i < InitParameter.KTimes.size(); i++) {
      for (int j = 0; j < InitParameter.nAnimals; j++) {
        sample<cell_as_newd> a(InitParameter, i, rr(generator));
        a.run();
        Result.push_back(a.get_result());
      }
    }
  } break;
  // symetric cell devision, daughters have same parameter but newly rolled
  case 2: {
    r_lognorm rr(InitParameter.Tc, InitParameter.sPopulation);
    for (unsigned i = 0; i < InitParameter.KTimes.size(); i++) {
      for (int j = 0; j < InitParameter.nAnimals; j++) {
        sample<cell_sym> a(InitParameter, i, rr(generator));
        a.run();
        Result.push_back(a.get_result());
      }
    }
  } break;
  // asymetric cell devision, daughter has mother's parameter, cells are initilizes with linear age incearsment 
  case 99: {
    r_lognorm rr(InitParameter.Tc, InitParameter.sPopulation);
    for (unsigned i = 0; i < InitParameter.KTimes.size(); i++) {
      for (int j = 0; j < InitParameter.nAnimals; j++) {
        sample<cell_as> a(InitParameter, i, rr(generator),21);
        a.run();
        Result.push_back(a.get_result());
      }
    }
  } break;
  // asymetric cell division, daughter has newly rolled parameter
  case 1: {
    r_lognorm rr(InitParameter.Tc, InitParameter.sPopulation);
    for (unsigned i = 0; i < InitParameter.KTimes.size(); i++) {
      for (int j = 0; j < InitParameter.nAnimals; j++) {
        sample<cell_as_newd> a(InitParameter, i, rr(generator));
        a.run();
        Result.push_back(a.get_result());
      }
    }
  } break;
  // symetric cell devision, daughters have different parameter newly rolled
  case 3: {
    r_lognorm rr(InitParameter.Tc, InitParameter.sPopulation);
    for (unsigned i = 0; i < InitParameter.KTimes.size(); i++) {
      for (int j = 0; j < InitParameter.nAnimals; j++) {
        sample<cell_sym_uncorr> a(InitParameter, i, rr(generator));
        a.run();
        Result.push_back(a.get_result());
      }
    }
  } break;
 
  }

  return vectorToList_Float(Result);
  //
  // std::cout.flush();
}

static PyMethodDef moduleMethods[] = {
    {"run", run_wrapper, METH_VARARGS, "Say hello"}, {NULL, NULL, 0, NULL}};

static struct PyModuleDef cclasim_module =
{
    PyModuleDef_HEAD_INIT,
    "cclasim", /* name of module */
    "c simulation for labeling assays",          /* module documentation, may be NULL */
    -1,          /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
    moduleMethods
};


PyMODINIT_FUNC
PyInit_cclasim(void)
{
    return PyModule_Create(&cclasim_module);
}


