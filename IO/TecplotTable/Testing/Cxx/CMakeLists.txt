
vtk_add_test_cxx(vtkIOTecplotTableCxxTests tests
  NO_VALID
  TestTecplotTableReader.cxx
  )

ExternalData_add_test(VTKData
  NAME TestTecplotTableReader
  COMMAND vtkIOTecplotTableCxxTests TestTecplotTableReader
  -D DATA{../Data/residuals.dat}
  )

vtk_test_cxx_executable(vtkIOTecplotTableCxxTests tests)
