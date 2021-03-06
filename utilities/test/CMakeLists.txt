include(ALPSEnableTests)

set (test_src
    test_utils_test
    temporary_filename
    filename_operations
    gtest_par_xml_output
    inf
    type_traits
    vector_functions
    rectangularize
    )

set (test_src_mpi
    mpi_utils_env
    mpi_utils_comm
    mpi_utils_bcast
    )

foreach(test ${test_src})
    alps_add_gtest(${test})
endforeach(test)

if(ALPS_HAVE_MPI)
  foreach(test ${test_src_mpi})
    alps_add_gtest(${test} PARTEST NOMAIN)
  endforeach(test)
  alps_add_gtest(mpi_utils_env_exceptions1 PARTEST SRCS mpi_test_support)
  alps_add_gtest(mpi_utils_env_exceptions2 PARTEST SRCS mpi_test_support)
endif()
