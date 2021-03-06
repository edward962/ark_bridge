#!/usr/bin/env bash
# Software License Agreement (BSD) 
#
# Author    Dave Niewinski <dniewinski@clearpathrobotics.com>
# Copyright (c) 2017, Clearpath Robotics, Inc., All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification, are permitted provided that
# the following conditions are met:
# * Redistributions of source code must retain the above copyright notice, this list of conditions and the
#   following disclaimer.
# * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
#   following disclaimer in the documentation and/or other materials provided with the distribution.
# * Neither the name of Clearpath Robotics nor the names of its contributors may be used to endorse or
#   promote products derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
# PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
# TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

print_help() {
    if [ $# -gt 0 ] ; then
        echo -e "ERROR: $*\n"
    fi
    echo "Usage: $0 [options] configfile"
    echo "Generate C++ republisher code for LCM to ROS or ROS to LCM messaging."
    echo "Options:"
    echo "  -h      Print this help and exit"
    echo ""
    echo "This code uses an input config file. Each (non # commented) line "
    echo "defines the topic name, lcm message package, message type, and "
    echo "republish direction (ros2lcm or lcm2ros) as a comma-separated list."
    echo "Thus a sample file (including the first header line) looks like: "
    echo ""
    echo "  # Topic name, Message package, Message type, Direction "
    echo "  example_topic, exlcm, example_t, lcm2ros"
    echo ""
    echo "Using this configuration would generate the C++ code for a republisher"
    echo "that reads messages of type 'exlcm::example_t' from LCM channel"
    echo "'example_topic', and publishes the equivalent message onto the ROS "
    echo "topic 'example_topic'."
    exit 2
}

# Process option flags
while getopts ':h' flag; do
  case "${flag}" in
    h) print_help ;;
    \?) print_help "Invalid option: -$OPTARG" ;;
  esac
done

# Remove accepted options (now config file argument should be $1)
shift $((OPTIND-1))

if [ $# -eq 0 ] ; then
    print_help "Must specify input config file"
fi

mkdir -p launch autosrc
touch -a autosrc/CMakeLists.txt

# Create (empty) launch file
LAUNCH_FILE=${1##*/}                # Remove leading directories
LAUNCH_FILE=${LAUNCH_FILE%.*}       # Remove trailing extension
LAUNCH_FILE="launch/${LAUNCH_FILE}.launch"
echo "Adding publishers to launch file: ${LAUNCH_FILE} (will be overwritten)" >&2
# if [ ! -f $LAUNCH_FILE ] ; then
    echo  "<launch>" > $LAUNCH_FILE
    echo -e "\t<master auto=\"start\" />" >> $LAUNCH_FILE
    echo -e "\t<group ns=\"ark_bridge\">" >>  $LAUNCH_FILE
    echo -e "\t</group>" >>  $LAUNCH_FILE
    echo "</launch>" >> $LAUNCH_FILE
# fi

# Generate republishers
while IFS=", " read TOPIC_NAME PACKAGE_NAME MESSAGE_TYPE DIRECTION JUNK ; do
    if [ -z "$TOPIC_NAME" ] || [ $( echo ${TOPIC_NAME} | head -c 1 ) = "#" ] ; then
        continue
    fi

    echo "Processing - Topic: ${TOPIC_NAME}, Message: ${PACKAGE_NAME}/${MESSAGE_TYPE}, Direction: ${DIRECTION}" >&2

    # Check which direction specified
    if [ "$DIRECTION" = lcm2ros ] ; then
        RFILE=src/lcm2ros_default_republisher.cpp.in
    elif [ "$DIRECTION" = ros2lcm ] ; then
        RFILE=src/ros2lcm_default_republisher.cpp.in
    else
        echo -e "\tERROR: Direction ${DIRECTION} not recognised,  must be 'ros2lcm' or 'lcm2ros'" >&2
        continue
    fi

    # Check if specified message exists
    if [ ! -f "${PACKAGE_NAME}/${MESSAGE_TYPE}.hpp" ] ; then
        echo -e "\tERROR: Message type "${PACKAGE_NAME}/${MESSAGE_TYPE}.hpp" not found." >&2
        continue
    fi

    # Generate republisher code
    OUTFILE="autosrc/${TOPIC_NAME}_republisher.cpp"
    echo -n -e "\tCreating CPP file $OUTFILE..." >&2
    echo $(printf '/%.0s' {1..71}) > $OUTFILE
    echo "// This source was automatically generated by the ark_bridge package" >>$OUTFILE
    echo -e "$(printf '/%.0s' {1..71})\n//" >>$OUTFILE
    echo "// Source message:    $MESSAGE_TYPE.msg" >> $OUTFILE
    echo "// Creation:          $(date '+%c')" >> $OUTFILE
    echo -e "//\n$(printf '/%.0s' {1..71})" >>$OUTFILE
    cat $RFILE | sed "s/@MESSAGE_TYPE@/$MESSAGE_TYPE/g" | \
        sed "s/@TOPIC_NAME@/$TOPIC_NAME/g; s/@PACKAGE_NAME@/$PACKAGE_NAME/g; " >> $OUTFILE
    echo " done." >& 2

    # If not already present, add CMakeLists.txt entry
    if ! grep -q "add_executable(\s*${TOPIC_NAME}_republisher" autosrc/CMakeLists.txt ; then
        echo -n -e "\tAdding entry to autosrc/CMakeLists.txt ..." >&2
        cat src/default_CMakeLists.txt.in | sed "s/@TOPIC_NAME@/$TOPIC_NAME/g" >> autosrc/CMakeLists.txt
        echo " done." >&2
    fi

    if ! grep -q "type=\"${TOPIC_NAME}_republisher" $LAUNCH_FILE ; then
        echo -n -e "\tAdding entry to $LAUNCH_FILE ..." >&2
        head -n-2 $LAUNCH_FILE > tmp
        echo -e "\t\t<node pkg=\"ark_bridge\" type=\"${TOPIC_NAME}_republisher\" respawn=\"true\" name=\"${TOPIC_NAME}_republisher\" output=\"screen\" />" >> tmp
        tail -n2 $LAUNCH_FILE >> tmp
        mv tmp $LAUNCH_FILE
        echo " done." >&2
    fi

done < "$1"
