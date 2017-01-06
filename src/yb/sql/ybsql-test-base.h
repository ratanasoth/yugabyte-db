//--------------------------------------------------------------------------------------------------
// Copyright (c) YugaByte, Inc.
//--------------------------------------------------------------------------------------------------

#ifndef YB_SQL_YBSQL_TEST_BASE_H_
#define YB_SQL_YBSQL_TEST_BASE_H_

#include "yb/sql/ybsql.h"
#include "yb/sql/session_context.h"

#include "yb/client/client.h"
#include "yb/integration-tests/mini_cluster.h"
#include "yb/master/mini_master.h"

#include "yb/util/test_util.h"

namespace yb {
namespace sql {

// Base class for all SQL test cases.
class YbSqlTestBase : public YBTest {
 public:
  //------------------------------------------------------------------------------------------------
  // Constructor and destructor.
  YbSqlTestBase();
  ~YbSqlTestBase();

  //------------------------------------------------------------------------------------------------
  // Test start and cleanup functions.
  virtual void SetUp() OVERRIDE {
    YBTest::SetUp();
  }
  virtual void TearDown() OVERRIDE {
    if (cluster_ != nullptr) {
      cluster_->Shutdown();
    }
    YBTest::TearDown();
  }

  //------------------------------------------------------------------------------------------------
  // Test only the parser.
  ErrorCode TestParser(const std::string& sql_stmt) {
    return ybsql_->TestParser(sql_stmt);
  }

  //------------------------------------------------------------------------------------------------
  // Create simulated cluster.
  void CreateSimulatedCluster();

  // Create a session context for client_.
  SessionContext *CreateSessionContext(std::shared_ptr<client::YBSession> session = nullptr);

  // Pull a session from the cached tables.
  SessionContext *GetSessionContext(int session_id);

  // Create a session context for a new connection.
  SessionContext *CreateNewConnectionContext();

 protected:
  //------------------------------------------------------------------------------------------------
  static constexpr int kSessionTimeoutMs = 60000;

  // SQL engine.
  YbSql::UniPtr ybsql_;

  // Simulated cluster.
  std::shared_ptr<MiniCluster> cluster_;

  // Simulated YB client.
  std::shared_ptr<client::YBClient> client_;

  // Contexts to be passed to SQL engine.
  std::vector<SessionContext::UniPtr> session_contexts_;
};

}  // namespace sql
}  // namespace yb

#endif  // YB_SQL_YBSQL_TEST_BASE_H_