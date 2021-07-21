<div class="lds-css ng-scope">
    <div style="width:100%; height:100%; margin: auto;" class="lds-magnify">
        <div>
            <div>
                <div></div>
                <div></div>
            </div>
        </div>
    </div>
    <style type="text/css">
      @keyframes lds-magnify {
        0% {
          -webkit-transform: translate(2px, 2px);
          transform: translate(2px, 2px);
        }
        33.33% {
          -webkit-transform: translate(102px, 2px);
          transform: translate(102px, 2px);
        }
        66.66% {
          -webkit-transform: translate(42px, 102px);
          transform: translate(42px, 102px);
        }
        100% {
          -webkit-transform: translate(2px, 2px);
          transform: translate(2px, 2px);
        }
      }

      @-webkit-keyframes lds-magnify {
        0% {
          -webkit-transform: translate(2px, 2px);
          transform: translate(2px, 2px);
        }
        33.33% {
          -webkit-transform: translate(102px, 2px);
          transform: translate(102px, 2px);
        }
        66.66% {
          -webkit-transform: translate(42px, 102px);
          transform: translate(42px, 102px);
        }
        100% {
          -webkit-transform: translate(2px, 2px);
          transform: translate(2px, 2px);
        }
      }

      .lds-magnify {
        position: relative;
      }

      .lds-magnify > div {
        -webkit-transform: scale(0.8);
        transform: scale(0.8);
        -webkit-transform-origin: 100px 100px;
        transform-origin: 100px 100px;
      }

      .lds-magnify > div > div {
        -webkit-animation: lds-magnify 2s linear infinite;
        animation: lds-magnify 2s linear infinite;
        position: absolute;
      }

      .lds-magnify > div > div div:nth-child(1) {
        width: 96px;
        height: 96px;
        border-radius: 50%;
        border: 12px solid #8f4928;
        background: #fffaeb;
      }

      .lds-magnify > div > div div:nth-child(2) {
        width: 17px;
        height: 51px;
        -webkit-transform: rotate(-45deg);
        transform: rotate(-45deg);
        background: #8f4928;
        border-radius: 0 0 8px 8px;
        position: absolute;
        top: 84px;
        left: 108px;
      }

      .lds-magnify {
        width: 64px !important;
        height: 64px !important;
        -webkit-transform: translate(-32px, -32px) scale(0.32) translate(32px, 32px);
        transform: translate(-32px, -32px) scale(0.32) translate(32px, 32px);
      }
    </style>
</div>
