from launch import LaunchDescription
from launch.substitutions import EnvironmentVariable
import launch_ros.actions

def generate_launch_description():
    ld = LaunchDescription([
        launch_ros.actions.Node(
            name='server_node',
            package='action_exception_test',
            executable='action_exception_test_server_node',
            output='screen'),
        launch_ros.actions.Node(
            name='client_node',
            package='action_exception_test',
            executable='action_exception_test_client_node_multithread_reentrant',
            output='screen'),
    ])
    return ld
