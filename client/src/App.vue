<template>
  <div id="app">
    <v-container style="margin-left: 250px" grid-list-md justify-space-around>
      <v-layout row class="mt-3 ml-2">
        <v-flex xs12 sm6 md2>
          <center>
            <img src="./assets/logo.png" width="160">
          </center>
        </v-flex>

        <v-flex xs12 sm6 md5 class="mt-5 ml-4">
          <v-text-field
            v-model="filePath"
            class="disabled"
            label="Solo"
            placeholder="enter file path"
            solo
          ></v-text-field>
        </v-flex>
        <v-flex xs12 sm6 md6 class="ml-5 mt-5">
          <v-btn
            class="uploadFile"
            :disabled="isUploaded"
            @click="uploadFile"
            color="primary lime"
          >upload</v-btn>
          <v-btn class="clear" color="info amber" :disabled="isClearDisabled" @click="onClear">clear</v-btn>
          <v-btn
            class="start"
            color="info deep-orange"
            :disabled="isStartDisabled"
            @click="onStart"
          >start</v-btn>
          <br>
          <form class="ml-2 mt-2">
            <v-layout row wrap>
            <v-flex md5>
              <v-text-field v-model="selectFilePath" class=" info" @click="$refs.inputChooseFile.click()" placeholder="select file to upload" readonly></v-text-field>
              <input v-show="false" ref="inputChooseFile" type="file" style="width:205px" @change="getFile($event)">
            </v-flex>
            <v-flex md3>
              <v-btn class="red info" @click="submitForm($event)" round>Submit</v-btn>
            </v-flex>
            </v-layout>
          </form>
        </v-flex>
      </v-layout>

      <v-layout row wrap>
        <v-flex xs12 sm6 md2 class="mt-5 pt-4">
          <center>
            <span v-if="this.pathNum !== -1" class="title">We have found {{ pathNum }} best paths!</span>
            <br>
            <br>
            <span v-if="this.pathNum !== -1" class="title">Now PATH
              <span class="orange--text" style="font-weight:bold">{{ this.pathRouteIndex + 1 }}</span> will be shown.
            </span>
            <v-menu offset-y open-on-hover>
              <template v-slot:activator="{ on }">
                <v-btn
                  :disabled="isSelectPathDisabled"
                  color="brown"
                  dark
                  v-on="on"
                  class="mt-4"
                >select path</v-btn>
              </template>
              <v-list>
                <v-list-tile
                  v-for="(path, index) in pathMenu"
                  :key="index"
                  z-index="9999"
                  @click="changePath(index)"
                  color="brown"
                >
                  <v-list-tile-title class="body-2">{{ path.title }}</v-list-tile-title>
                </v-list-tile>
              </v-list>
            </v-menu>
          </center>
        </v-flex>
        <v-flex xs12 sm6 md5>
          <div id="grid" class>
            <svg :width="width" :height="height" style="position: absolute">
              <g
                class="x axis"
                transform="translate(10,15)"
                fill="none"
                font-size="10"
                font-family="sans-serif"
                text-anchor="middle"
              >
                <template v-for="x in xaxis">
                  <g
                    class="tick"
                    opacity="1"
                    :key="x.x + 'xaxis'"
                    :style="{ transform: 'translate(' + x.x * length + 'px,' + 0 + 'px)' }"
                  >
                    <text fill="#000">{{x.x}}</text>
                  </g>
                </template>
              </g>
              <g
                class="y axis"
                transform="translate(2,25)"
                fill="none"
                font-size="10"
                font-family="sans-serif"
                text-anchor="middle"
              >
                <template v-for="y in yaxis">
                  <g
                    class="tick"
                    opacity="1"
                    :key="y.y + 'yaxis'"
                    :style="{ transform: 'translate(' + 0 + 'px,' + y.y * length + 'px)' }"
                  >
                    <text fill="#000">{{y.y}}</text>
                  </g>
                </template>
              </g>
              <g class="row" transform="translate(10,20)">
                <template v-for="rect in rects">
                  <rect
                    :key="rect.index"
                    class="square"
                    :style="{x: rect.x, y: rect.y, width: length, height: length}"
                    style="fill: rgb(255, 255, 255); stroke: grey;"
                  ></rect>
                </template>
              </g>
            </svg>
            <svg
              :width="width"
              :height="height"
              style=" position: absolute"
              version="1.1"
              xmlns="http://www.w3.org/2000/svg"
              xmlns:xlink="http://www.w3.org/1999/xlink"
            >
              <defs>
                <marker
                  id="arrow"
                  markerUnits="strokeWidth"
                  markerWidth="5"
                  markerHeight="5"
                  viewBox="0 0 10 10"
                  refX="6"
                  refY="6"
                  orient="auto"
                >
                  <path d="M2,2 L10,6 L2,10 L6,6 L2,2" :style="{fill: currentLineColor}"></path>
                </marker>
              </defs>

              <g ref="circles" id="circles" transform="translate(10,20)">
                <template v-for="line in lines">
                  <line
                    :stroke="line.color"
                    stroke-width="4"
                    :key="line.index"
                    :x1="line.x1"
                    :y1="line.y1"
                    :x2="line.x2"
                    :y2="line.y2"
                    marker-end="url(#arrow)"
                  ></line>
                  <!-- marker-end="url(#arrow)" -->
                </template>
                <template v-for="point in points">
                  <circle
                    :key="point.index"
                    :class="
                      (point.xIndex === xIndex) && (point.yIndex === yIndex)? 'now' :
                      (point.isStart === true)? 'start': 
                      (point.isCustomer === true)? 'customer':
                      (point.isVisited === true? 'visited' : 'point')"
                    :style="{ transform: 'translate('+point.x+'px,'+point.y+'px)' }"
                  ></circle>
                </template>
                <image
                  v-if="this.xIndex !== null"
             
                  xlink:href="./assets/car.png"
                  :x="(this.yIndex + 1 ) * 60 - 47"
                  :y="(this.xIndex + 1 ) * 60 - 47"
                  height="90px"
                  width="90px"
                ></image>

                <template v-for="(customer,index) in customers">
                  <image
                    :key="index"
                    xlink:href="./assets/customer.png"
                    :x="(customer.x + 1)* 60 - 13"
                    :y="(customer.y + 1)* 60 - 15"
                    height="30px"
                    width="30px"
                  ></image>
                </template>
              </g>
            </svg>
          </div>
        </v-flex>

        <v-flex md3>
          <v-card class="pl-2 mt-4">
            <v-card-title class="title">Details</v-card-title>
            <v-divider></v-divider>
            <v-card-text>
              <v-flex xs6 sm12>
                <span class="subheading font-weight-medium">nodeIndex:&nbsp&nbsp&nbsp
                  <input
                    style="width:100px;font-weight:bold;text-decoration:underline;"
                    v-model="this.pathIndex"
                  >
                </span>
                <br>
                <br>
                <span class="subheading font-weight-medium">currentTime:&nbsp&nbsp&nbsp
                  <input
                    style="width:60px;font-weight:bold;color:red;text-decoration:underline;"
                    v-model="this.currentTime"
                  >
                </span>(s)
                <br>
                <br>
                <span class="subheading font-weight-medium">
                  x:&nbsp&nbsp&nbsp
                  <input
                    style="width:80px;font-weight:bold;text-decoration:underline;"
                    v-model="xIndex"
                  >
                  y:&nbsp&nbsp&nbsp
                  <input
                    style="width:60px;text-decoration:underline;"
                    v-model="yIndex"
                  >
                </span>
                <br>
                <br>

                <br>
                <span class="subheading mt-2">pathTrack:</span>
                <br>
                <br>
                <span
                  class="subheading pb-1 orange--text darken--4--text"
                  style="line-height: 1.5;font-weight:bold;text-decoration:underline;"
                >{{ pathTrack }}</span>
              </v-flex>
              <br>
            </v-card-text>
          </v-card>
        </v-flex>
      </v-layout>
      <router-view/>
    </v-container>
  </div>
</template>

<style>
body {
  font-family: Helvetica;
  font-size: 10px;
  background-image: url("assets/background.png");
  background-size: 100%;
  background-position: center bottom;
  background-attachment: fixed;
}
.point {
  fill: #222;
  r: 3;
}
.customer {
  fill: red;
  r: 4;
}
.start {
  fill: #e65100;
  r: 8;
}
.now {
  fill: orange;
  r: 4;
}
.visited {
  fill: black;
  r: 2;
}
</style>

<script src="https://d3js.org/d3-path.v1.min.js"></script>

<script>
import * as d3 from "d3";
import axios from "axios";
import FileUpload from "vue-simple-upload/dist/FileUpload";
import json2txt from "txt2json";

export default {
  name: "App",
  components: {
    fileupload: FileUpload
  },
  data() {
    return {
      grid: "",
      data: [99, 71, 78, 25, 36, 92],
      line: "",
      width: 0,
      height: 0,
      points: [],
      rects: [],
      lines: [],
      length: 20,
      filePath: "",
      resultFilePath: "",
      isUploaded: false,
      path: [],
      cost: 0,
      xIndex: null,
      yIndex: null,
      timer: "",
      value: 0,
      pathIndex: 0,
      isClearDisabled: true,
      isStartDisabled: true,
      xaxis: [],
      yaxis: [],
      customerSum: 0,
      customers: [],
      pathTrack: "",
      N: 0,
      M: 0,
      startPoint: [],
      currentTime: 0,
      intersections: [],
      clock: 500,
      pathMenu: [],
      pathRouteIndex: 0,
      isSelectPathDisabled: true,
      lineColors: [],
      currentLineColor: "",
      pathNum: -1,
      selectFilePath: ''
    };
  },
  mounted() {
    //this.draw()
  },
  methods: {
    getFile(event) {
      this.file = event.target.files[0]
      this.selectFilePath = event.target.files[0].name
      console.log(this.file)
    },
    submitForm(event) {
      event.preventDefault();
      let formData = new FormData();

      formData.append("file", this.file);

      let config = {
        headers: {
          "Content-Type": "multipart/form-data"
        }
      };

      axios
        .post("http://0.0.0.0:5000/input",  this.filePath)
        .then(() => {
          console.log("post")
          this.getInputData();
        })
        .catch(error => {
          alert("error when posting data");
          console.log(error);
        });
    },
    getXAxisData(N, length) {
      var data = new Array();
      for (var row = 0; row <= N; row = row + 5) {
        data.push({
          x: row
        });
      }
      return data;
    },
    getYAxisData(N, length) {
      var data = new Array();
      for (var row = 0; row <= N; row = row + 5) {
        data.push({
          y: row
        });
      }
      return data;
    },
    getGridData(N, M, length) {
      var data = new Array();
      var xpos = 1; //starting xpos and ypos at 1 so the stroke will show when we make the grid below
      var ypos = 1;
      var width = length;
      var height = length;
      N += 1;
      M += 1;
      // iterate for rows
      for (var row = 0; row < N; row++) {
        // iterate for cells/columns inside rows
        for (var column = 0; column < M; column++) {
          data.push({
            x: xpos,
            y: ypos,
            width: width,
            height: height
          });
          // increment the x position. I.e. move it over by 50 (width variable)
          xpos += width;
        }
        // reset the x position after a row is complete
        xpos = 1;
        // increment the y position for the next row. Move it down 50 (height variable)
        ypos += height;
      }
      return data;
    },
    getPointData(N, M, length) {
      var data = new Array();
      var xpos = length + 1; //starting xpos and ypos at 1 so the stroke will show when we make the grid below
      var ypos = length + 1;
      var width = length;
      var height = length;

      // iterate for rows
      for (var row = 0; row < N; row++) {
        // iterate for cells/columns inside rows
        for (var column = 0; column < M; column++) {
          data.push({
            x: xpos,
            y: ypos,
            xIndex: row,
            yIndex: column,
            isCustomer: false,
            isVisited: false,
            isStart: false
          });
          // increment the x position. I.e. move it over by 50 (width variable)
          xpos += width;
        }
        // reset the x position after a row is complete
        xpos = length + 1;
        // increment the y position for the next row. Move it down 50 (height variable)
        ypos += height;
      }
      return data;
    },
    getRandomColor() {
      var r = Math.floor(Math.random() * 255);
      var g = Math.floor(Math.random() * 255);
      var b = Math.floor(Math.random() * 255);
      var color = "rgba(" + r + "," + g + "," + b + ",0.8)";
      return color;
    },
    deliveryGo() {
      console.log(this.path);
      this.currentLineColor = this.lineColors[this.pathRouteIndex];

      if (this.xIndex !== null && this.yIndex !== null) {
        this.points[this.xIndex * this.M + this.yIndex].isVisited = true;
        this.lines.push({
          x1: (this.yIndex + 1) * this.length + 1,
          y1: (this.xIndex + 1) * this.length + 1,
          x2:
            (this.path[this.pathRouteIndex][this.pathIndex].x + 1) *
              this.length +
            1,
          y2:
            (this.path[this.pathRouteIndex][this.pathIndex].y + 1) *
              this.length +
            1,
          color: this.lineColors[this.pathRouteIndex]
        });
        console.log(
          "x2, y2:",
          this.path[this.pathRouteIndex][this.pathIndex].x + 1,
          this.path[this.pathRouteIndex][this.pathIndex].x + 1
        );
      }

      this.currentTime = this.path[this.pathRouteIndex][this.pathIndex].time;

      console.log("this.pathIndex: " + this.pathIndex);
      this.xIndex = this.path[this.pathRouteIndex][this.pathIndex].y;
      this.yIndex = this.path[this.pathRouteIndex][this.pathIndex].x;
      this.pathIndex++;

      if (this.pathTrack !== "") this.pathTrack = this.pathTrack + " => ";

      this.pathTrack =
        this.pathTrack + "(" + this.yIndex + " , " + this.xIndex + ")";

      console.log(this.path[this.pathRouteIndex].length);
      if (this.pathIndex >= this.path[this.pathRouteIndex].length) {
        clearInterval(this.timer);
        this.isClearDisabled = false;
        this.isUploaded = false;
      }
    },
    getOutputData() {
      axios
        .get("http://0.0.0.0:5000/output")
        .then(res => {
          var data = res.data.output
            .split("\n")
            .join(" ")
            .split(" ")
            .map(Number);
          alert("Get ouput data successfully!");
          this.cost = data[0];
          this.tempData = [];
          var i = 0,
            start = 1,
            pathNum = 0;
          while (i + start < data.length) {
            this.path.push(new Array());
            for (
              ;
              i + start < data.length && data[i + start] != this.cost;
              i += 3
            ) {
              if(data[i + start] && data[i + start + 1] && data[i + start + 2])
              this.path[pathNum].push({
                x: data[i + start],
                y: data[i + 1 + start],
                time: data[i + 2 + start]
              });
            }
            this.lineColors.push(this.getRandomColor());
            console.log("start", start);
            console.log("i", i);
            start = i + start + 1;
            i = 0;
            pathNum++;
            var tempPath = "SHOW PATH" + pathNum;
            this.pathMenu.push({ title: tempPath });
          }
          this.pathNum = pathNum;

          console.log("menu: ", this.pathMenu);
          //this.pathNum = (i * 2 + 1)
          console.log("pathNum:", this.pathNum);
          console.log("path:", this.path);

          /*for(let j = 0; j < this.pathNum; j ++){
            console.log("upload")
            for(let i = 0 ; i < this.path[j].length; i ++)
              console.log(this.path[j][i].x , this.path[j][i].y)
          }*/

          this.filePath = "";

          this.isClearDisabled = true;
          this.isStartDisabled = false;
          this.isSelectPathDisabled = false;

          //REMENBER TO DELETE THIS    beautiful color
          ///
          ///
          this.lineColors[0] = "orange";
          this.lineColors[1] = "#01579B";
          this.lineColors[2] = "#4CAF50";
        })
        .catch(error => {
          // eslint-disable-next-line
          alert("error when getting output");
          console.error(error);
        });
    },
    getInputData(input) {
        var data = input
          .split("\n")
          .join(" ")
          .split(" ")
          .map(Number);

        this.N = data[0];
        this.M = data[1];
        this.startPoint.x = data[2];
        this.startPoint.y = data[3];

        var customerSum = data[4];

        this.customers = [];
        for (var i = 5; i < 5 + customerSum * 2; i = i + 2) {
          this.customers.push({
            x: data[i],
            y: data[i + 1]
          });
        }

        console.log("customers:  " + this.customers);

        var interBaseArray = data.slice(5 + customerSum * 2);

        let len = interBaseArray.length;
        let n = 5;
        let lineNum = len % n === 0 ? len / n : Math.floor(len / n + 1);
        this.intersection = [];
        for (let i = 0; i < lineNum; i++) {
          let temp = interBaseArray.slice(i * n, i * n + n);
          this.intersection.push(temp);
        }
        console.log("intersection: ", this.intersection);

        this.intersection.sort(function(x, y) {
          if (x[0] !== y[0]) return x[0] - y[0];
          else return x[1] - y[1];
        });

        console.log("sorted intersection: ", this.intersection);

        if (this.N < 20 && this.M < 20) this.length = 60;
        else if (this.N < 50 && this.M < 50) this.length = 30;
        else if (this.N < 50 && this.M < 50) this.length = 15;
        else this.length = 10;

        console.log("this.customers", this.customers);

        let N = this.N;
        let M = this.M;

        console.log(this.xIndex + " " + this.yIndex);
        this.width = (N + 3) * this.length;
        this.height = (M + 3) * this.length;
        //console.log('N: '+ N +'   M: ' + M)

        //console.log(customerSum)

        this.points = this.getPointData(N, M, this.length);
        for (var i = 0; i < customerSum; i++) {
          //console.log("customerData:" , this.customers[i].y - 1,this.M,this.customers[i].x - 1,(this.customers[i].y - 1)* this.M + this.customers[i].x - 1)
          this.points[
            this.customers[i].y * this.M + this.customers[i].x
          ].isCustomer = true;
        }

        this.points[
          this.startPoint.y * this.M + this.startPoint.x
        ].isStart = true;

        this.rects = this.getGridData(N, M, this.length);

        this.xaxis = this.getXAxisData(N, length);
        //console.log("xaxis: " + this.xaxis)

        this.yaxis = this.getYAxisData(M, length);
        //console.log("yaxis: " + this.yaxis)

        this.onClear();
        this.path = [];
        //this.getOutputData();

    },
    uploadFile() {
      this.pathMenu = [];
      console.log("filePath", this.filePath);
      axios
        .post("http://0.0.0.0:5000/input", {filePath: this.filePath})//, {filePath: this.filePath})
        .then((res) => {
          console.log(res.data)

          this.getInputData(res.data.input)

          var data = res.data.output
            .split("\n")
            .join(" ")
            .split(" ")
            .map(Number);
          alert("Get ouput data successfully!");
          this.cost = data[0];
          this.tempData = [];
          var i = 0,
            start = 1,
            pathNum = 0;
          while (i + start < data.length && pathNum < 5) { //not to show more
            this.path.push(new Array());
            for (
              ;
              i + start < data.length && data[i + start] != this.cost;
              i += 3
            ) {
              if(data[i + start] != undefined && data[i + start + 1] != undefined && data[i + start + 2] != undefined)
              this.path[pathNum].push({
                x: data[i + start],
                y: data[i + 1 + start],
                time: data[i + 2 + start]
              });
            }
            this.lineColors.push(this.getRandomColor());
            console.log("start", start);
            console.log("i", i);
            start = i + start + 1;
            i = 0;
            pathNum++;
            var tempPath = "SHOW PATH" + pathNum;
            this.pathMenu.push({ title: tempPath });
          }
          this.pathNum = pathNum;

          console.log("menu: ", this.pathMenu);
          //this.pathNum = (i * 2 + 1)
          console.log("pathNum:", this.pathNum);
          console.log("path:", this.path);

          /*for(let j = 0; j < this.pathNum; j ++){
            console.log("path all:")
            for(let i = 0 ; i < this.path[j].length; i ++)
              console.log(this.path[j][i].x , this.path[j][i].y)
          }*/

          this.filePath = "";

          this.isClearDisabled = true;
          this.isStartDisabled = false;
          this.isSelectPathDisabled = false;

          //constraint line color to beautiful colors
          this.lineColors[0] = "orange";
          this.lineColors[1] = "#01579B";
          this.lineColors[2] = "#4CAF50";

        })
        .catch(error => {
          alert("Error when posting data! Try to input 'datafile.txt'!");
          console.log(error);
        });
    },
    changePath(val) {
      this.onClear();
      //console.log("menuindex", val)

      this.pathRouteIndex = val;
      /*for(let i = 0 ; i < this.path[this.pathRouteIndex].length; i ++)
        console.log(this.path[this.pathRouteIndex][i].x , this.path[this.pathRouteIndex][i].y)*/
      console.log("timer", this.timer);
      clearInterval(this.timer);
      this.onStart();
    },
    onClear() {
      this.isUploaded = false;
      this.pathIndex = 0;
      this.xIndex = null;
      this.yIndex = null;
      this.pathTrack = "";
      this.lines = [];
      for (var row = 0; row < this.N; row++)
        for (var col = 0; col < this.M; col++)
          this.points[row * this.M + col].isVisited = false;
      this.isStartDisabled = false;

      this.currentTime = 0;
    },
    onStart() {
      this.isUploaded = true;

      this.timer = setInterval(this.deliveryGo, this.clock);
      this.isStartDisabled = true;
    }
  }
};
</script>
