#include "itkImageFileWriter.h"

#include "itkSmoothingRecursiveGaussianImageFilter.h"

#include "itkPluginUtilities.h"

#include "USRFHighPassFilterCLP.h"

#include "vtkImageButterworthHighPass.h"
#include "vtkImageFFT.h"
#include "vtkImageRFFT.h"
#include "vtkImageCast.h"
#include "itkImageToVTKImageFilter.h"
#include "itkVTKImageToImageFilter.h"
#include "vtkSmartPointer.h"
#include "vtkImageExtractComponents.h"


//Large portions adapted from: http://www.vtk.org/Wiki/VTK/Examples/Cxx/Images/ImageIdealHighPass



int main( int argc, char * argv[] )
{
  PARSE_ARGS;

  const unsigned int Dimension = 2;

  typedef short InputPixelType;
  typedef itk::Image<InputPixelType, Dimension> InputImageType;

  typedef double FFTPixelType;
  typedef itk::Image<FFTPixelType, Dimension> FFTImageType;

  typedef short OutputPixelType;
  typedef itk::Image<OutputPixelType, Dimension> OutputImageType;

  typedef itk::ImageToVTKImageFilter<InputImageType> ImageToVTKType;
  typedef itk::VTKImageToImageFilter<OutputImageType> VTKToImageType;

  typedef itk::ImageFileReader<InputImageType> ReaderType;
  typedef itk::ImageFileWriter<OutputImageType> WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputVolume.c_str());
  reader->Update(); 

  ImageToVTKType::Pointer inputConverter = ImageToVTKType::New();
  inputConverter->SetInput(reader->GetOutput());
  inputConverter->Update();

  vtkSmartPointer<vtkImageCast> originalCastFilter =
    vtkSmartPointer<vtkImageCast>::New();
  originalCastFilter->SetInputData(inputConverter->GetOutput());
  originalCastFilter->SetOutputScalarTypeToShort();
  originalCastFilter->Update();
    
  // Compute the FFT of the image
  vtkSmartPointer<vtkImageFFT> fftFilter =
    vtkSmartPointer<vtkImageFFT>::New();
  fftFilter->SetInputConnection(originalCastFilter->GetOutputPort());
  fftFilter->Update();

  // High pass filter the FFT
  vtkSmartPointer<vtkImageButterworthHighPass> highPassFilter =
    vtkSmartPointer<vtkImageButterworthHighPass>::New();
  highPassFilter->SetInputConnection(fftFilter->GetOutputPort());
  highPassFilter->SetXCutOff(0.1);
  highPassFilter->SetYCutOff(0.05);
  highPassFilter->Update();

  // Compute the IFFT of the high pass filtered image
  vtkSmartPointer<vtkImageRFFT> rfftFilter =
    vtkSmartPointer<vtkImageRFFT>::New();
  rfftFilter->SetInputConnection(highPassFilter->GetOutputPort());
  rfftFilter->Update();

  vtkSmartPointer<vtkImageExtractComponents> extractRealFilter =
    vtkSmartPointer<vtkImageExtractComponents>::New();
  extractRealFilter->SetInputConnection(rfftFilter->GetOutputPort());
  extractRealFilter->SetComponents(0);
  extractRealFilter->Update();

  // Cast the output back to short
  vtkSmartPointer<vtkImageCast> outputCastFilter =
    vtkSmartPointer<vtkImageCast>::New();
  outputCastFilter->SetInputConnection(extractRealFilter->GetOutputPort());
  outputCastFilter->SetOutputScalarTypeToShort();
  outputCastFilter->Update();

  // convert back to itk

  VTKToImageType::Pointer convertToImage = VTKToImageType::New();
  convertToImage->SetInput(outputCastFilter->GetOutput());
  convertToImage->Update();

  //set up writer
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputVolume.c_str());
  writer->SetInput(convertToImage->GetOutput());
  writer->Update();
  
  return EXIT_SUCCESS;
}
