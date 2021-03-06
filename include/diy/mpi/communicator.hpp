namespace diy
{
namespace mpi
{

  //! \ingroup MPI
  //! Simple wrapper around `MPI_Comm`.
  class communicator
  {
    public:
                inline
                communicator(MPI_Comm comm = MPI_COMM_WORLD);

      int       rank() const                        { return rank_; }
      int       size() const                        { return size_; }

      //void      send(int dest,
      //               int tag,
      //               const void* buf,
      //               MPI_Datatype datatype) const   { }

      //! Send `x` to processor `dest` using `tag` (blocking).
      template<class T>
      void      send(int dest, int tag, const T& x) const   { detail::send<T>()(comm_, dest, tag, x); }

      //! Receive `x` from `dest` using `tag` (blocking).
      //! If `T` is an `std::vector<...>`, `recv` will resize it to fit exactly the sent number of values.
      template<class T>
      status    recv(int source, int tag, T& x) const       { return detail::recv<T>()(comm_, source, tag, x); }

      //! Non-blocking version of `send()`.
      template<class T>
      request   isend(int dest, int tag, const T& x) const  { return detail::isend<T>()(comm_, dest, tag, x); }

      //! Non-blocking version of `recv()`.
      //! If `T` is an `std::vector<...>`, its size must be big enough to accomodate the sent values.
      template<class T>
      request   irecv(int source, int tag, T& x) const      { return detail::irecv<T>()(comm_, source, tag, x); }

      //! probe
      inline
      status    probe(int source, int tag) const;

      //! iprobe
      inline
      optional<status>
                iprobe(int source, int tag) const;

      //! barrier
      inline
      void      barrier() const;

                operator MPI_Comm() const                   { return comm_; }

    private:
      MPI_Comm  comm_;
      int       rank_;
      int       size_;
  };
}
}

diy::mpi::communicator::
communicator(MPI_Comm comm)
:comm_(comm), rank_(0), size_(1)
{
#ifndef DIY_NO_MPI
  if (comm != MPI_COMM_NULL)
  {
    MPI_Comm_rank(comm_, &rank_);
    MPI_Comm_size(comm_, &size_);
  }
#endif
}

diy::mpi::status
diy::mpi::communicator::
probe(int source, int tag) const
{
  (void) source;
  (void) tag;

#ifndef DIY_NO_MPI
  status s;
  MPI_Probe(source, tag, comm_, &s.s);
  return s;
#else
  DIY_UNSUPPORTED_MPI_CALL(MPI_Probe);
#endif
}

diy::mpi::optional<diy::mpi::status>
diy::mpi::communicator::
iprobe(int source, int tag) const
{
  (void) source;
  (void) tag;
#ifndef DIY_NO_MPI
  status s;
  int flag;
  MPI_Iprobe(source, tag, comm_, &flag, &s.s);
  if (flag)
    return s;
#endif
  return optional<status>();
}

void
diy::mpi::communicator::
barrier() const
{
#ifndef DIY_NO_MPI
  MPI_Barrier(comm_);
#endif
}
