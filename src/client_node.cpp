#include <action_client_test/client_node.h>

ClientNode::ClientNode()
  : Node("action_test_client_node")
  , cb_group_reentrant_(create_callback_group(rclcpp::callback_group::CallbackGroupType::Reentrant))
  , client_(rclcpp_action::create_client<Fibonacci>(this, "/fibonacci", cb_group_reentrant_))
  , timer_(create_wall_timer(std::chrono::duration<double>(1.0), std::bind(&ClientNode::sendGoal, this)))
{
}

void ClientNode::sendGoal()
{
  RCLCPP_INFO(get_logger(), "Sending goal");

  Fibonacci::Goal goal;
  goal.order = 10;

  rclcpp_action::Client<Fibonacci>::SendGoalOptions options;
  options.result_callback = [](const ClientGoalHandleFibonacci::WrappedResult& result) { (void) result; };

  auto gh_future = client_->async_send_goal(goal, options);
  gh_future.wait();

  auto async_res_future = gh_future.get()->async_result();
  async_res_future.wait();

  auto result = async_res_future.get();

  if (result.code != rclcpp_action::ResultCode::SUCCEEDED)
  {
    RCLCPP_WARN(get_logger(), "Action did not succeed");
    return;
  }

  std::stringstream stream;
  for (auto& seq : result.result->sequence)
  {
    stream << seq << " ";
  }

  RCLCPP_INFO_STREAM(get_logger(), "Action succeeded! " << stream.str());
}

int main(int argc, char* argv[])
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<ClientNode>();
  rclcpp::executors::MultiThreadedExecutor executor;
  executor.add_node(node);
  executor.spin();
  executor.remove_node(node);
  rclcpp::shutdown();
}
