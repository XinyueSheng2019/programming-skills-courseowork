import subprocess
import pytest
import os
import filecmp

def test_valid():

	cmd = "make"
	subprocess.call(cmd, shell = True)
	
	#test with user-defined parameters
	cmd = "./percolate -l 40 -r 0.3 -s 500 -m 50 -d test.dat -g test.pgm > output_1.txt" 
	file1 = 'output_1.txt'
	file2 = 'test.dat'
	file3 = 'test.pgm'
	test_file1 = 'testoracle/valid_case/user_define_paras/output_1.txt'
	test_file2 = 'testoracle/valid_case/user_define_paras/test.dat'
	test_file3 = 'testoracle/valid_case/user_define_paras/test.pgm'
	check(cmd,file1,file2,file3,test_file1,test_file2,test_file3)
	

	#test with user-defined parameters when l == 1
	cmd = "./percolate -l 1 > output_2.txt" 
	file1 = 'output_2.txt'
	file2 = 'map.dat'
	file3 = 'map.pgm'
	test_file1 = 'testoracle/valid_case/user_define_paras/output_2.txt'
	test_file2 = 'testoracle/valid_case/user_define_paras/map.dat'
	test_file3 = 'testoracle/valid_case/user_define_paras/map.pgm'
	check(cmd,file1,file2,file3,test_file1,test_file2,test_file3)
	


	#test with default parameters
	cmd = "./percolate > output.txt" 
	file1 = 'output.txt'
	file2 = 'map.dat'
	file3 = 'map.pgm'
	test_file1 = 'testoracle/valid_case/default_paras/output.txt'
	test_file2 = 'testoracle/valid_case/default_paras/map.dat'
	test_file3 = 'testoracle/valid_case/default_paras/map.pgm'
	check(cmd,file1,file2,file3,test_file1,test_file2,test_file3)
	
def test_invalid():

	#when l is invalid
	cmd = "./percolate -l -1 -r 0.3 -s 500 -m 50 -d test.dat -g test.pgm > output_l.txt" 
	execute_status(cmd)
	file_name1 = 'output_l.txt'
	file_name2 = 'testoracle/invalid_case/output_l.txt'
	compare_txt_files(file_name1,file_name2)

	#when rho is invalid (<0)
	cmd = "./percolate -l 40 -r -1 -s 500 -m 50 -d test.dat -g test.pgm > output_r1.txt" 
	execute_status(cmd)
	file_name1 = 'output_r1.txt'
	file_name2 = 'testoracle/invalid_case/output_r1.txt'
	compare_txt_files(file_name1,file_name2)

	#when rho is invalid (>1)
	cmd = "./percolate -l 40 -r 2 -s 500 -m 50 -d test.dat -g test.pgm > output_r2.txt" 
	execute_status(cmd)
	file_name1 = 'output_r2.txt'
	file_name2 = 'testoracle/invalid_case/output_r2.txt'
	compare_txt_files(file_name1,file_name2)

	#when s is invalid (<0)
	cmd = "./percolate -l 40 -r 0.3 -s -1 -m 50 -d test.dat -g test.pgm > output_s1.txt" 
	execute_status(cmd)
	file_name1 = 'output_s1.txt'
	file_name2 = 'testoracle/invalid_case/output_s1.txt'
	compare_txt_files(file_name1,file_name2)

	#when s is invalid (> 900000000)
	cmd = "./percolate -l 40 -r 0.3 -s 900000002 -m 50 -d test.dat -g test.pgm > output_s2.txt" 
	execute_status(cmd)
	file_name1 = 'output_s2.txt'
	file_name2 = 'testoracle/invalid_case/output_s2.txt'
	compare_txt_files(file_name1,file_name2)

	#when m is invalid (<0)
	cmd = "./percolate -l 40 -r 0.3 -s 500 -m -1 -d test.dat -g test.pgm > output_m1.txt" 
	execute_status(cmd)
	file_name1 = 'output_m1.txt'
	file_name2 = 'testoracle/invalid_case/output_m1.txt'
	compare_txt_files(file_name1,file_name2)

	#when m is invalid (> l*l)
	cmd = "./percolate -l 40 -r 0.3 -s 500 -m 1800 -d test.dat -g test.pgm > output_m2.txt" 
	execute_status(cmd)
	file_name1 = 'output_m2.txt'
	file_name2 = 'testoracle/invalid_case/output_m2.txt'
	compare_txt_files(file_name1,file_name2)

	cmd = "make clean"
	subprocess.call(cmd, shell = True)


def check(cmd,file1,file2,file3,test_file1,test_file2,test_file3):
	execute_status(cmd)
	#test output files exit
	judge_exist(file1)
	judge_exist(file2)
	#compare output information with expected infomation in testoracle files
	compare_txt_files(file1, test_file1)
	# compare the output dat file with expected file in testoracle file folder
	compare_map_files(file2, test_file2)
	# compare the output pgm file with expected file in testoracle file folder
	compare_map_files(file3, test_file3)

def execute_status(cmd):
	result = subprocess.call(cmd, shell = True)
	assert 0 == result, "Unexpected execution status"

def judge_exist(file):
	assert os.path.isfile(file),"could not find " + file + " file"

def compare_txt_files(file_name1, file_name2):
	result = 0
	with open(file_name1) as file1, open(file_name2) as file2:
		for line1,line2 in zip(file1,file2):
			if line1 != line2:
				result = 1
				break
	file1.close()
	file2.close()
	assert 0 == result, "Actual ouput file does not match the expected file!"

def compare_map_files(file_name1, file_name2):
	result = filecmp.cmp(file_name1,file_name2)
	assert True == result, "Actual " +file_name1 + " file does not match the expected file!"

















