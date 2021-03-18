#include <stdlib.h>
#include <cv_bridge/cv_bridge.h>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <opencv4/opencv2/opencv.hpp>
#include <memory>
#include <string>

using std::placeholders::_1;

class ImageSaver : public rclcpp::Node
{
public:
  ImageSaver(const std::string & node_name)
  : Node(node_name), current_img_(NULL), clock_(RCL_SYSTEM_TIME)
  {
    this->declare_parameter<int>("freq", 5);
    this->declare_parameter<std::string>("image_topic", "/xtion/rgb/image_raw");
    this->declare_parameter<std::string>("prefix_name", "");

    initParams();

    image_sub_ =
      this->create_subscription<sensor_msgs::msg::Image>(
      image_topic_, 1,
      std::bind(&ImageSaver::imageCb, this, _1));
  }

  void
  run()
  {
    rclcpp::Rate loop_rate(freq_);
    while (rclcpp::ok()) {
      rclcpp::spin_some(this->get_node_base_interface());
      this->step();
      loop_rate.sleep();
    }
  }

private:
  bool
  rosImg2cvImg(cv::Mat & cv_img_out)
  {
    cv_bridge::CvImagePtr cv_img_ptr;

    try {
      cv_img_ptr = cv_bridge::toCvCopy(
        current_img_, current_img_->encoding);
    } catch (cv_bridge::Exception & e) {
      RCLCPP_ERROR(get_logger(), "cv_bridge exception: %s", e.what());
      return false;
    }

    cv_img_out = cv_img_ptr->image;
    return true;
  }

  void
  step()
  {
    if (current_img_ == NULL) {
      return;
    }

    // ros2 image to cv::Mat

    cv::Mat cv_img;

    if (!rosImg2cvImg(cv_img)) {
      return;
    }

    // save the image

    saveImg(cv_img);
  }

  void
  saveImg(const cv::Mat & cv_img)
  {
    std::string file_name;
    long time_ns;

    time_ns = clock_.now().nanoseconds();

    file_name = prefix_name_ + "_" + std::to_string(time_ns) + ".jpg";
    cv::imwrite(file_name, cv_img);
  }

  void
  imageCb(const sensor_msgs::msg::Image::SharedPtr msg)
  {
    current_img_ = msg;
  }

  void
  initParams()
  {
    this->get_parameter("freq", freq_);
    this->get_parameter("image_topic", image_topic_);
    this->get_parameter("prefix_name", prefix_name_);
  }

  std::string image_topic_, prefix_name_;
  int freq_;
  sensor_msgs::msg::Image::SharedPtr current_img_;
  rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr image_sub_;
  rclcpp::Clock clock_;
};

int
main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);

  auto image_saver_node = std::make_shared<ImageSaver>("image_saver_node");

  image_saver_node->run();

  rclcpp::shutdown();
  exit(EXIT_SUCCESS);
}