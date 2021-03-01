
var table = function () {
  function sorter(n) {
    this.n = n; this.t; this.b; this.r; this.d; this.p; this.w; this.a = []; this.l = 0
  }
  sorter.prototype.init = function (t, f) {
    this.t = document.getElementById(t);
    this.b = this.t.getElementsByTagName('tbody')[0];
    this.r = this.b.rows; var l = this.r.length;
    for (var i = 0; i < l; i++) {
      if (i == 0) {
        var c = this.r[i].cells; this.w = c.length;
        for (var x = 0; x < this.w; x++) {
          if (c[x].className != 'nosort') {
            if (c[x].className == 'vsort') {
              c[x].setAttribute('sort_attr', 'vsort');
            } else if (c[x].className == 'lsort') {
              c[x].setAttribute('sort_attr', 'lsort');
            } else if (c[x].className == 'gsort') {
              c[x].setAttribute('sort_attr', 'gsort');
            } else if (c[x].className == 'csort') {
              c[x].setAttribute('sort_attr', 'csort');
            } else if (c[x].className == 'rsort') {
              c[x].setAttribute('sort_attr', 'rsort');
            }
            c[x].className = 'head';
            c[x].onclick = new Function(this.n + '.work(this.cellIndex)')
          }
        }
      } else {
        this.a[i - 1] = {}; this.l++;
      }
    }
    if (f != null) {
      var a = new Function(this.n + '.work(' + f + ')'); a()
    }
  }
  sorter.prototype.work = function (y) {
    this.b = this.t.getElementsByTagName('tbody')[0]; this.r = this.b.rows;
    var x = this.r[0].cells[y], i;
    for (i = 0; i < this.l; i++) {
      this.a[i].o = i + 1;
      var v = this.r[i + 1].cells[y].firstChild;
      if ( v != null && (v.nodeName == 'A' || v.nodeName == 'B' || v.nodeName == 'P') ) {
        v = v.firstChild
      }
      this.a[i].value = (v != null) ? v.nodeValue : ''
    }
    for (i = 0; i < this.w; i++) {
      var c = this.r[0].cells[i];
      if (c.className != 'nosort') { c.className = 'head' }
    }
    if (this.p == y) {
      this.a.reverse();
      x.className = (this.d) ? 'asc' : 'desc';
      this.d = (this.d) ? false : true
    } else {
      this.p = y;
      var xattr = x.getAttribute('sort_attr');
      if (xattr == 'vsort') {
        this.a.sort(vcompare);
      } else if (xattr == 'lsort') {
        this.a.sort(lcompare);
      } else if (xattr == 'gsort') {
        this.a.sort(gcompare);
        } else if (xattr == 'csort') {
        this.a.sort(ccompare);
      } else {
        this.a.sort(compare);
      }
      x.className = 'asc';
      this.d = false
    }
    var n = document.createElement('tbody');
    n.appendChild(this.r[0]);
    for (i = 0; i < this.l; i++) {
      var r = this.r[this.a[i].o - 1].cloneNode(true);
      n.appendChild(r); r.className = (i % 2 == 0) ? 'even' : 'odd'
    }
    this.t.replaceChild(n, this.b)
  }
  function compare(f, c) {
    f = f.value.toLowerCase(), c = c.value.toLowerCase();
    var i = parseFloat(f.replace(/(\$|\,)/g, '')), n = parseFloat(c.replace(/(\$|\,)/g, ''));
    if (!isNaN(i) && !isNaN(n)) { f = i, c = n }
    return (f > c ? 1 : (f < c ? -1 : 0))
  }
  function vcompare(f, c) {
    f = f.value, c = c.value;
    var i = parseFloat(f.replace('V', '')), n = parseFloat(c.replace('V', ''));
    if (!isNaN(i) && !isNaN(n)) { f = i, c = n }
    return (f > c ? 1 : (f < c ? -1 : 0))
  }
  function lcompare(f, c) {
    f = f.value, c = c.value;
    var i = parseFloat(levelIndex(f)), n = parseFloat(levelIndex(c));
    if (!isNaN(i) && !isNaN(n)) { f = i, c = n }
    return (f > c ? 1 : (f < c ? -1 : 0))
  }
  function gcompare(f, c) {
    f = f.value, c = c.value;
    var i = parseFloat(groupIndex(f)), n = parseFloat(groupIndex(c));
    if (!isNaN(i) && !isNaN(n)) { f = i, c = n }
    return (f > c ? 1 : (f < c ? -1 : 0))
  }
  function ccompare(f, c) {
    f = f.value, c = c.value;
    var i = parseFloat(f.replace('CWE-', '')), n = parseFloat(c.replace('CWE-', ''));
    if (!isNaN(i)) { f = i; }
    else { f = Number.MAX_VALUE; }
    if (!isNaN(n)) { c = n; }
    else { c = Number.MAX_VALUE; }
    return (f > c ? 1 : (f < c ? -1 : 0))
  }
  function levelIndex(lvl) {
    if (lvl.startsWith('High')) {
      return 1;
    } else if (lvl.startsWith('Medium')) {
      return 2;
    } else if (lvl.startsWith('Low')) {
      return 3;
    } else {
      return 0;
    }
  }
  function groupIndex(group) {
    if (group.startsWith('General Analysis')) {
      return 1;
    } else if (group.startsWith('Micro-optimizations')) {
      return 2;
    } else if (group.startsWith('64-bit errors')) {
      return 3;
    } else if (group.startsWith('Customers Specific')) {
      return 4;
    } else {
      return 0;
    }
  }
  return { sorter: sorter }
}();
