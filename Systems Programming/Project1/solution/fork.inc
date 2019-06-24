 Points hull1;
  Points hull2;
  int fd[2];  //pipe file descruptor 

  pid_t p; 

  if (pipe(fd)==-1) 
  { 
      fprintf(stderr, "Pipe Failed" ); 
      exit(EXIT_FAILURE); 
  } 

  p = fork(); 

  if (p < 0)
  { 
      fprintf(stderr, "fork Failed" ); 
      exit(EXIT_FAILURE);
  } 
  else if (p > 0) //parent
  { 
    close(fd[1]);// Close the writing end 

    Slice slice1{points, (size + 1) / 2};
    slice1.divide_and_conquer((n_processors + 1) / 2, hull1);

    // Wait for the child to exist
    int status;
    waitpid(p, &status, 0);

    if(WEXITSTATUS(status) == 1)//child existed with failure
    {
      cout << "Child exited with a failure!" << endl;
      exit(EXIT_FAILURE);
    }

    //read the results
    hull2.read_raw(fd[0]);

    close(fd[0]); // Close writing end of second pipe 
  } 
  else//child
  { 
    // Close the reading end
    close(fd[0]);

    Slice slice2{points + (size + 1) / 2, size / 2};
    slice2.divide_and_conquer(n_processors / 2, hull2);

    //write the results
    hull2.write_raw(fd[1]);

    // Close the writing ends 
    close(fd[1]);  

    exit(EXIT_SUCCESS);
  } 