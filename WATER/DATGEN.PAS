uses crt;

type    pbuf = ^buf;
        buf = array[0..64000] of byte;

        tb  = record
                c   : byte;
                pos : word;
              end;

var    r,r1,sc,
       g,b  : pbuf;
       f      : file;
       x,y,c  : word;
       row    : array[0..320*3-1] of byte;
       tc     : word;
       tbuf   : array[0..100] of word;
       pal    : array[0..768] of byte;

       tmp    : array[0..20000] of tb;

procedure setrgb(c,r,g,b:byte);
begin
     port[$3c8] := c;
     port[$3c9] := r;
     port[$3c9] := g;
     port[$3c9] := b;
end;

var  ty,tt,ab  : word;


procedure zeta(n1,n2,n3:string);
begin
     assign(f,n1);
     reset(f,1);
     seek(f,8);
     for y := 0 to 199 do
     begin
      blockread(f,row,320*3);
      for x := 0 to 319 do r^[x+y*320] := row[x*3+0];
     end;
     close(f);
     assign(f,n2);
     reset(f,1);
     seek(f,8);
     for y := 0 to 199 do
     begin
      blockread(f,row,320*3);
      for x := 0 to 319 do r1^[x+y*320] := row[x*3+0];
     end;
     close(f);
     assign(f,n3);
     rewrite(f,1);

{     move(r1^,mem[$a000:0],64000);}

     for y := 34 downto 1 do
     begin
       tc := 0;
       for x := 0 to 64000 do
         if (sc^[x] <> 40) and
            (r^[x] <> r1^[x]) and
            (g^[x] = y) then
         begin
           tmp[tc].c   := r^[x];
           tmp[tc].pos := x;
           sc^[x] := 40;
           mem[$a000:x] := 10;
           inc(tc);
         end;

       ab := tc-1;
       for tt := 1 to 158 do
       begin
       tc := 0;
        for ty := 0 to ab do
         begin
          if tmp[ty].c = tt then
           begin
            mem[$a000:tmp[ty].pos] := 40;
            tbuf[tc] := tmp[ty].pos;
            inc(tc);
           end;
         end;
       blockwrite(f,tc,2);
       if tc > 0 then blockwrite(f,tbuf,tc*2);
       end;
     end;
     close(f);

end;

begin
     getmem(r,64000);
     getmem(r1,64000);
     getmem(g,64000);
     getmem(b,64000);
     getmem(sc,64000);
     fillchar(r^,64000,#0);
     fillchar(r1^,64000,#0);
     fillchar(g^,64000,#0);
     fillchar(b^,64000,#0);
     fillchar(sc^,64000,#0);

     fillchar(tmp,sizeof(tmp),#0);

     asm
     mov   ax,$13
     int   $10
     end;


     { Load fixed green mask }
     assign(f,'green.clx');
     reset(f,1);
     seek(f,778);
     blockread(f,g^,64000);
     close(f);

{
     assign(f,'o:k1.mtv');
     reset(f,1);
     seek(f,8);
     for y := 0 to 199 do
     begin
      blockread(f,row,320*3);
      for x := 0 to 319 do  mem[$a000:x+y*320] := row[x*3+1];
     end;
     close(f);
}

     zeta('o:k1.mtv','o:k2.mtv','o:wat1.dat');
     zeta('o:k2.mtv','o:k3.mtv','O:wat2.dat');
     zeta('o:k3.mtv','o:k5.mtv','O:wat3.dat');
{     zeta('o:k4.mtv','o:k5.mtv','O:wat4.dat');}

     repeat until keypressed;

     asm
     mov   ax,$3
     int   $10
     end;

     freemem(r,64000);
     freemem(r1,64000);
     freemem(g,64000);
     freemem(b,64000);
     freemem(sc,64000);
end.