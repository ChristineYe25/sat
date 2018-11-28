#include <cmath>
#include <vector>
#include "xcl2.hpp"

using std::cout;
using std::endl;
using std::string;
using std::vector;


#define NUM_CLAUSES 1065
#define NUM_TEST 1

// Util functions for host
void read_clause_file(string filename,
    vector<int, aligned_allocator<int>> &c1,
    vector<int, aligned_allocator<int>> &c2,
    vector<int, aligned_allocator<int>> &c3);


int main(int argc, char** argv)
{
  //Allocate Memory in Host Memory
  size_t vector_size_bytes = sizeof(int) * NUM_CLAUSES;

  vector<int, aligned_allocator<>> source_in1(NUM_CLAUSES);
  vector<int, aligned_allocator<int>> source_in2(NUM_CLAUSES);
  vector<int, aligned_allocator<int>> source_in3(NUM_CLAUSES);

  vector<int, aligned_allocator<int>> hw_results(1);

  read_clause_file("../data/uf250-01.cnf", source_in1,source_in2,source_in3);
  hw_results[0] = 0;


//OPENCL HOST CODE AREA START
  vector<cl::Device> devices = xcl::get_xil_devices();
  cl::Device device = devices[0];

  cl::Context context(device);
  cl::CommandQueue q(context, device, CL_QUEUE_PROFILING_ENABLE);
  string device_name = device.getInfo<CL_DEVICE_NAME>(); 

  string binaryFile = xcl::find_binary_file(device_name, "dot_prod_kernel");
  cl::Program::Binaries bins = xcl::import_binary_file(binaryFile);
  devices.resize(1);
  cl::Program program(context, devices, bins);
  cl::Kernel kernel(program, "dot_prod_kernel");

  //Allocate Buffer in Global Memory
  vector<cl::Memory> inBufVec, outBufVec;
  cl::Buffer buffer_in1(context,CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY,
                        vector_size_bytes, source_in1.data());
  cl::Buffer buffer_in2(context,CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, 
                        vector_size_bytes, source_in2.data());
  cl::Buffer buffer_in2(context,CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, 
                        vector_size_bytes, source_in3.data());

  cl::Buffer buffer_output(context,CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, 
                           sizeof(int), hw_results.data());

  inBufVec.push_back(buffer_in1);
  inBufVec.push_back(buffer_in2);
  inBufVec.push_back(buffer_in3);
  outBufVec.push_back(buffer_output);

  //Copy input data to device global memory
  q.enqueueMigrateMemObjects(inBufVec, 0/* 0 means from host*/);

  auto krnl_solver = cl::KernelFunctor<cl::Buffer&, cl::Buffer&,
                                           cl::Buffer&, cl::Buffer&>(kernel);

  //Launch the Kernel
  NUM_NON_SAT = 0; 
  for (int i = 0; i < NUM_TEST; ++i) {
    krnl_solver(cl::EnqueueArgs(q, cl::NDRange(1, 1, 1), cl::NDRange(1, 1, 1)),cl::NDRange(1, 1, 1),
                  buffer_in1, buffer_in2, buffer_in3, buffer_output);

    //Copy Result from Device Global Memory to Host Local Memory
    q.enqueueMigrateMemObjects(outBufVec, CL_MIGRATE_MEM_OBJECT_HOST);
    q.finish();
    if (hw_results != 1){
        NUM_NON_SAT ++;
    }
  }
//OPENCL HOST CODE AREA END
  print ("Unsatisified cases: " + NUM_NON_SAT); 

}
