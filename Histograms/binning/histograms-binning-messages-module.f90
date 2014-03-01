! file: histograms-binning-messages-module.f90
! author: Bozhin Karaivanov
! date created: 15.02.2014
! last modified: 15.02.2014

module histograms_binning_messages_module

   public :: print_help, invalid_argument, io_error

contains

   subroutine print_help
   write (unit=0, fmt="('Usage: histograms <maxval> [<length> ]<infile> <outfile>')")
   write (unit=0, fmt="('where:')")
   write (unit=0, fmt="('    maxval = maximal value of the avalanche''s parameter')")
   write (unit=0, fmt="('    length = number of different values of the avalanche''s parameter')")
   write (unit=0, fmt="('             lenght must be less or equal to maxval')")
   write (unit=0, fmt="('    infile = name of the input file')")
   write (unit=0, fmt="('   outfile = name of the output file')")
   stop
   end subroutine print_help


   subroutine invalid_argument(ierr)
   write (unit=0, fmt="('ERROR: invalid argument!',/,'Error message #',i5)") ierr
   stop 'Invalid argument'
   end subroutine invalid_argument


   subroutine io_error(icode, ierr)
   if (icode.eq.0) then
      write (unit=0,fmt="('ERROR: Open input file! Error message #',i5)") ierr
      stop 'open input file'
   elseif (icode.eq.1) then
      write (unit=0,fmt="('ERROR: Open output file! Error message #',i5)") ierr
      stop 'open output file'
   elseif (icode.eq.2) then
      write (unit=0,fmt="('ERROR: Read input data! Error message #',i5)") ierr
      stop 'read data'
   elseif (icode.eq.3) then
      write (unit=0,fmt="('ERROR: Write data to output file! Error message #',i5)") ierr
      stop 'write data'
   endif
   end subroutine io_error

end module histograms_binning_messages_module

