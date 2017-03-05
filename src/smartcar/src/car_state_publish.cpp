    #include <string>  
    #include <ros/ros.h>  
    #include <sensor_msgs/JointState.h>  
    #include <tf/transform_broadcaster.h>  
    #include <nav_msgs/Odometry.h>
      
    int main(int argc, char** argv) {  
        ros::init(argc, argv, "state_publisher");  
        ros::NodeHandle n;  
        ros::Publisher joint_pub = n.advertise<sensor_msgs::JointState>("joint_states", 1);  
        ros::Publisher odom_pub=n.advertise<nav_msgs::Odometry>("odom",10);
        
        
        tf::TransformBroadcaster broadcaster;  
        ros::Rate loop_rate(30);  
      
        const double degree = M_PI/180;  
        const double radius = 2;  
      
     
        double angle= 0;  
      
        geometry_msgs::TransformStamped odom_trans;  
        sensor_msgs::JointState joint_state;  
        nav_msgs::Odometry odom;
        
        
        odom_trans.header.frame_id = "odom";  
        odom_trans.child_frame_id = "base_link";  
        ros::Time last,current;
         
        double vx=0.0;
        double vy=0.0;
        double vth=0.0;
        double dt=0.0;
      
        while (ros::ok()) {  
        	current=ros::Time::now();
            //update joint_state  
            joint_state.header.stamp = current;  
            joint_state.name.resize(4);  
            joint_state.position.resize(4);  
        joint_state.name[0] ="base_to_wheel1";  
            joint_state.position[0] = 0;  
        joint_state.name[1] ="base_to_wheel2";  
            joint_state.position[1] = 0;  
        joint_state.name[2] ="base_to_wheel3";  
            joint_state.position[2] = 0;  
        joint_state.name[3] ="base_to_wheel4";  
            joint_state.position[3] = 0;  
      
            // update transform  
            // (moving in a circle with radius)  
            odom_trans.header.stamp = current;  
            odom_trans.transform.translation.x = radius * cos(angle);  
            odom_trans.transform.translation.y = radius * sin(angle);  
            odom_trans.transform.translation.z = 0.0;  
            odom_trans.transform.rotation = tf::createQuaternionMsgFromYaw(angle);  
      
      	    //update odom
      	    odom.header.stamp=ros::Time::now();
      	    odom.header.frame_id="odom";
      	    
      	    
      	    odom.pose.pose.position.x=radius * cos(angle);
      	    odom.pose.pose.position.y=radius * sin(angle);
            odom.pose.pose.position.y=0.0;
            odom.pose.pose.orientation=tf::createQuaternionMsgFromYaw(angle);
            
            dt=(current-last).toSec();
            vx=radius * cos(angle)/dt;
            vy=radius * sin(angle)/dt;
            vth=degree/4/dt;
            
            odom.child_frame_id="base_link";
            odom.twist.twist.linear.x=vx;
            odom.twist.twist.linear.y=vy; 
            odom.twist.twist.angular.z=vth;
      
      
            //send the joint state and transform  
            joint_pub.publish(joint_state);  
            broadcaster.sendTransform(odom_trans);  
            odom_pub.publish(odom);
      
            // Create new car angle  
            angle += degree/4;  
      
            // This will adjust as needed per iteration  
            loop_rate.sleep();  
        }  
      
        return 0;  
    }  
