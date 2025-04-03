#ifndef MPIMANAGER_LIBRARY_H
#define MPIMANAGER_LIBRARY_H

#include <fmt/core.h>
#include <fmt/color.h>
#include <fmt/chrono.h>
#include <chrono>
#include <mpi.h>
#include <string>
#include <ranges>
#include <vector>

/*!
 * level enum according to the Syslog standard
 */
enum class Level
{
  emerg,
  alert,
  crit,
  err,
  warning,
  notice,
  info,
  debug,
};

/*!
 * rank enum used to specify if all ranks are to log or just rank zero
 */
enum class Ranks
{
  zero,
  all,
};

/*!
 * container for managing timers
 */
struct Timer
{
  /// timer start time
  std::chrono::time_point<std::chrono::system_clock> start;

  /// timer level
  Level level;

  /// timer name
  std::string name;
};

class MPIManager
{
public:
  /*!
   * constructs MPI environment
   * @param argc command line argument count
   * @param argv command line argument vector
   * @param level highest level to log
   * @param ranks ranks to log on
   */
  MPIManager(int& argc, char** argv, Level level, Ranks ranks);

  /*!
   * destructs MPI environment
   */
  ~MPIManager();

  /*!
   * aborts MPI on all ranks and displays error msg at emergency level
   * @param msg error message
   */
  void abort(const std::string& msg);

  /*!
   * logs msg to terminal at specified level and ranks
   * @param level Syslog Level to log at
   * @param msg message to log
   */
  void log(Level level, const std::string& msg);

  /*!
   * starts a timer
   * @param level timer level
   * @param name timer name
   */
  void timer_start(Level level, const std::string& name);

  /*!
   * stops most recently started timer
   */
  void timer_stop();

  /// MPI communicator
  MPI_Comm comm;

  /// rank within MPI communicator
  int rank = -1;

  /// size of MPI communicator
  int size = -1;

private:
  /*!
   * logs msg at emergency level
   * @param msg message to log
   */
  void log_emerg(const std::string& msg);

  /*!
   * logs msg at alert level
   * @param msg message to log
   */
  void log_alert(const std::string& msg);

  /*!
   * logs msg at critical level
   * @param msg message to log
   */
  void log_crit(const std::string& msg);

  /*!
   * logs msg at error level
   * @param msg message to log
   */
  void log_err(const std::string& msg);

  /*!
   * logs msg at warning level
   * @param msg message to log
   */
  void log_warning(const std::string& msg);

  /*!
   * logs msg at notice level
   * @param msg message to log
   */
  void log_notice(const std::string& msg);

  /*!
   * logs msg at the info level
   * @param msg
   */
  void log_info(const std::string& msg);

  /*!
   * logs msg at debug level
   * @param msg message to log
   */
  void log_debug(const std::string& msg);

  /*!
   * checks to see if desired level is low enough to take action given level set in constructor
   * @param level level to check
   * @return boolean stating if level is low enough
   */
  [[nodiscard]] bool sufficient_level(Level level) const;

  /*!
   * checks to see if this rank should take action given ranks set in constructor
   * @return boolean stating if action should be taken on this rank
   */
  [[nodiscard]] bool sufficient_rank() const;

  /// highest level to log at
  const Level level;

  /// ranks to log on
  const Ranks ranks;

  /// stack of timers
  std::vector<Timer> timers;
};

#endif //MPIMANAGER_LIBRARY_H
