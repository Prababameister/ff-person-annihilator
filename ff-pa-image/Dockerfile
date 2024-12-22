ARG ROS_DISTRO=jazzy
FROM osrf/ros:${ROS_DISTRO}-desktop-full

ENV ROS_DISTRO=${ROS_DISTRO}

ENV DISPLAY=novnc:0.0
ENV DOCKER_CONTAINER

RUN apt-get -o Acquire::Check-Valid-Until=false -o Acquire::Check-Date=false update

RUN apt-get install -y git unzip wget python3-pip vim clang libclang-dev
RUN apt-get install -y ros-${ROS_DISTRO}-ros-gz \
                       ros-${ROS_DISTRO}-xacro \
                       ros-${ROS_DISTRO}-ros2-control \
                       ros-${ROS_DISTRO}-ros2-controllers \
                       ros-${ROS_DISTRO}-joint-state-publisher \
                       ros-${ROS_DISTRO}-joint-state-publisher-gui \
                       ros-${ROS_DISTRO}-gz-ros2-control \
                       ros-${ROS_DISTRO}-control-toolbox \
                       ros-${ROS_DISTRO}-robot-localization \
                       ros-${ROS_DISTRO}-mavros \
                       ros-${ROS_DISTRO}-navigation2 \
                       ros-${ROS_DISTRO}-behaviortree-cpp \
                       ros-${ROS_DISTRO}-image-common

RUN pip install cmake-language-server --break-system-packages

RUN git clone https://github.com/MaskRay/ccls.git /ccls && \
    cd /ccls && \
    git submodule update --init && \
    cmake -H. -BRelease -DCMAKE_BUILD_TYPE=Release && \
    cmake --build Release --target install && \
    cd /

CMD ["bash"]
