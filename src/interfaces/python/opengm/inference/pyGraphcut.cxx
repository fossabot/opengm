#include <boost/python.hpp>
#include <string>
#include "inf_def_visitor.hxx"

#include <opengm/inference/graphcut.hxx>
#include <opengm/inference/alphabetaswap.hxx>
#include <opengm/inference/alphaexpansion.hxx>
#ifdef WITH_BOOST
#  include <opengm/inference/auxiliary/minstcutboost.hxx>
#endif
#ifdef WITH_MAXFLOW
#  include <opengm/inference/auxiliary/minstcutkolmogorov.hxx>
#endif

# include <param/graph_cut_param.hxx>




using namespace boost::python;

template<class GM,class ACC>
void export_graphcut(){
   import_array(); 
   typedef GM PyGm;
   typedef typename PyGm::ValueType ValueType;
   typedef typename PyGm::IndexType IndexType;
   
   append_subnamespace("solver");


   #ifdef WITH_MAXFLOW
      const bool withMaxFlow=true;
   #else
      const bool withMaxFlow=false;
   #endif
   

   // documentation 
   InfSetup setup;
   setup.cite       = "";
   setup.algType    = "graphCut";
   setup.guarantees = "optimal ";
   setup.limitations= "max 2.order, binary labels, must be submodular";
   setup.hyperParameterKeyWords = StringVector(1,std::string("minStCut"));
   setup.hyperParametersDoc     = StringVector(1,std::string("minStCut implementation of graphcut"));
   setup.dependencies = "to use ``'kolmogorov'`` as minStCut the kolmogorov max flow library, " 
                        "compile OpenGM with CMake-Flag ``WITH_CPLEX`` set to ``ON`` ";

   #ifdef WITH_MAXFLOW
      // set up hyper parameter name for this template
      setup.isDefault=withMaxFlow;
      setup.hyperParameters= StringVector(1,std::string("kolmogorov"));
      typedef opengm::external::MinSTCutKolmogorov<size_t,ValueType> MinStCutKolmogorov;
      typedef opengm::GraphCut<PyGm, ACC, MinStCutKolmogorov>        PyGraphCutKolmogorov;
      // export parameter
      exportInfParam<exportTag::NoSubInf,PyGraphCutKolmogorov>("_GraphCut_Kolmogorov");
      // export inference
      class_< PyGraphCutKolmogorov>("_GraphCut_Kolmogorov",init<const GM & >())  
      .def(InfSuite<PyGraphCutKolmogorov,false>(std::string("GraphCut"),setup))
      ;
   #endif


   // set up hyper parameter name for this template
   setup.isDefault=!withMaxFlow;
   setup.hyperParameters= StringVector(1,std::string("boost-kolmogorov"));
   typedef opengm::MinSTCutBoost<size_t, ValueType, opengm::KOLMOGOROV> MinStCutBoostKolmogorov;
   typedef opengm::GraphCut<PyGm, ACC, MinStCutBoostKolmogorov> PyGraphCutBoostKolmogorov;
   // export parameter
   exportInfParam<exportTag::NoSubInf,PyGraphCutBoostKolmogorov>("_GraphCut_Boost_Kolmogorov");
   // export inference
   class_< PyGraphCutBoostKolmogorov>("_GraphCut_Boost_Kolmogorov",init<const GM & >())  
   .def(InfSuite<PyGraphCutBoostKolmogorov,false>(std::string("GraphCut"),setup))
   ;

   // set up hyper parameter name for this template
   setup.isDefault=false;
   setup.hyperParameters= StringVector(1,std::string("push-relabel"));
   typedef opengm::MinSTCutBoost<size_t, ValueType, opengm::PUSH_RELABEL> MinStCutBoostPushRelabel;
   typedef opengm::GraphCut<PyGm, ACC, MinStCutBoostPushRelabel> PyGraphCutBoostPushRelabel;
   // export parameter
   exportInfParam<exportTag::NoSubInf,PyGraphCutBoostPushRelabel>("_GraphCut_Boost_Push_Relabel");
   // export inference
   class_< PyGraphCutBoostPushRelabel>("_GraphCut_Boost_Push_Relabel",init<const GM & >())  
   .def(InfSuite<PyGraphCutBoostPushRelabel,false>(std::string("GraphCut"),setup))
   ;


}

template void export_graphcut<GmAdder,opengm::Minimizer>();
//template void export_graphcut<GmAdder,opengm::Maximizer>();

